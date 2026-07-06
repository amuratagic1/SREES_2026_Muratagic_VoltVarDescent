#pragma once
#include <dss_capi.h>
#include <gui/Types.h>
#include <dense/Matrix.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <map>

// Wrapper around the dss_capi global API (v0.14.x, no context handles).
class DSSEngine
{
public:
    DSSEngine()
    {
        DSS_Start(0);
    }

    // Run a block of DSS commands (multi-line string).
    void commandBlock(const std::string& block)
    {
        Text_CommandBlock(block.c_str());
    }

    std::string lastError()
    {
        const char* s = Error_Get_Description();
        return s ? s : "";
    }

    struct Summary
    {
        int32_t buses;
        int32_t nodes;
        int32_t lines;
        int32_t loads;
        int32_t transformers;
        int32_t capacitors;
        int32_t generators;
        int32_t pvSystems;
        int32_t totalElements;
    };

    Summary getSummary()
    {
        return {
            Circuit_Get_NumBuses(),
            Circuit_Get_NumNodes(),
            Lines_Get_Count(),
            Loads_Get_Count(),
            Transformers_Get_Count(),
            Capacitors_Get_Count(),
            Generators_Get_Count(),
            PVSystems_Get_Count(),
            Circuit_Get_NumCktElements()
        };
    }

    // Per-node voltage magnitudes in pu: names in circuit order, vmag as a natID dense column vector.
    struct VoltageData
    {
        std::vector<std::string> names;
        dense::DblMatrix         vmag; // (names.size() x 1)
    };

    VoltageData getNodeVoltages()
    {
        VoltageData result;

        char** names = nullptr;
        int32_t nameDims = 0;
        Circuit_Get_AllNodeNames(&names, &nameDims);

        double* vmagRaw = nullptr;
        int32_t vDims = 0;
        Circuit_Get_AllBusVmagPu(&vmagRaw, &vDims);

        if (!names || !vmagRaw || nameDims <= 0) return result;
        int32_t n = std::min(nameDims, vDims);

        result.names.reserve(n);
        for (int32_t i = 0; i < n; i++)
            if (names[i]) result.names.push_back(names[i]);

        result.vmag = dense::DblMatrix(static_cast<td::UINT4>(result.names.size()), 1);
        auto w = result.vmag.getFirstColumnManipulator();
        td::UINT4 k = 0;
        for (int32_t i = 0; i < n; i++)
            if (names[i]) w(k++) = vmagRaw[i];

        return result;
    }

    std::vector<std::string> getPVNames()
    {
        std::vector<std::string> names;
        char** list = nullptr;
        int32_t dims = 0;
        PVSystems_Get_AllNames(&list, &dims);
        if (!list || dims <= 0) return names;
        for (int32_t i = 0; i < dims; i++)
            if (list[i]) names.push_back(list[i]);
        return names;
    }

    void setPVkvar(const std::string& name, double q)
    {
        PVSystems_Set_Name(name.c_str());
        PVSystems_Set_kvar(q);
    }

    static double voltageCost(const VoltageData& voltages)
    {
        double sum = 0.0;
        auto r = voltages.vmag.getFirstColumnManipulator();
        td::UINT4 n = voltages.vmag.getNoOfRows();
        for (td::UINT4 i = 0; i < n; i++)
        {
            double d = r(i) - 1.0;
            sum += d * d;
        }
        return sum;
    }

    static std::string busFromNode(const std::string& node)
    {
        auto dot = node.rfind('.');
        return (dot == std::string::npos) ? node : node.substr(0, dot);
    }

    // Groups per-node voltages by bus for display (last phase seen wins, as buses are ~balanced pu).
    static std::map<std::string, double> groupVoltagesByBus(const VoltageData& nodes)
    {
        std::map<std::string, double> buses;
        auto r = nodes.vmag.getFirstColumnManipulator();
        for (size_t i = 0; i < nodes.names.size(); i++)
            buses[busFromNode(nodes.names[i])] = r(static_cast<td::UINT4>(i));
        return buses;
    }

    static std::string voltageStatus(double v)
    {
        if (v < 0.95) return gui::tr("logLow").c_str();
        if (v > 1.05) return gui::tr("logHigh").c_str();
        return "";
    }

    static std::string padLeft(const std::string& s, size_t width)
    {
        if (s.size() >= width) return s.substr(s.size() - width);
        return std::string(width - s.size(), ' ') + s;
    }

    static std::string padRight(const std::string& s, size_t width)
    {
        if (s.size() >= width) return s.substr(0, width);
        return s + std::string(width - s.size(), ' ');
    }

    static std::string repeatChar(char c, size_t n)
    {
        return std::string(n, c);
    }

    static std::string formatDouble(double v, int precision, size_t width, bool scientific = false)
    {
        std::ostringstream s;
        if (scientific)
            s << std::scientific << std::setprecision(precision) << v;
        else
            s << std::fixed << std::setprecision(precision) << v;
        return padLeft(s.str(), width);
    }

    static std::string joinCols(const std::vector<std::string>& cols, size_t gap = 3)
    {
        std::string line;
        for (size_t i = 0; i < cols.size(); ++i)
        {
            if (i > 0) line += std::string(gap, ' ');
            line += cols[i];
        }
        return line;
    }

    static std::string formatCost(double c, size_t width = 14)
    {
        return formatDouble(c, 3, width, true);
    }

    static void appendVoltageTable(std::ostringstream& out, const char* title,
                                   const std::map<std::string, double>& busV)
    {
        const size_t wBus = 14;
        const size_t wV = 10;
        const size_t wStatus = 8;

        out << title << "\n";
        out << joinCols({
            padRight(gui::tr("logBus").c_str(), wBus),
            padLeft(gui::tr("logVPu").c_str(), wV),
            padRight(gui::tr("logStatus").c_str(), wStatus)
        }) << "\n";
        out << joinCols({
            repeatChar('-', wBus),
            repeatChar('-', wV),
            repeatChar('-', wStatus)
        }) << "\n";

        for (auto& kv : busV)
        {
            out << joinCols({
                padRight(kv.first, wBus),
                formatDouble(kv.second, 4, wV),
                padRight(voltageStatus(kv.second), wStatus)
            }) << "\n";
        }
        out << "\n";
    }

    struct IterRow
    {
        int it;
        double cost;
        dense::DblMatrix Q; // (pvNames.size() x 1) snapshot for this iteration
    };

    static bool shouldShowIter(int it, int lastIt)
    {
        if (it == 0 || it == lastIt) return true;
        if (it <= 5) return true;
        return it % 5 == 0;
    }

    static void appendIterationTable(std::ostringstream& out,
                                     const std::vector<std::string>& pvNames,
                                     const std::vector<IterRow>& rows,
                                     int convergedAt)
    {
        const size_t wIter = 6;
        const size_t wCost = 14;
        const size_t wQ = 14;

        out << gui::tr("logOptimization").c_str() << "\n";

        std::vector<std::string> header = {
            padLeft(gui::tr("logIter").c_str(), wIter),
            padLeft(gui::tr("logCost").c_str(), wCost)
        };
        std::vector<std::string> separator = {
            repeatChar('-', wIter),
            repeatChar('-', wCost)
        };
        for (auto& name : pvNames)
        {
            header.push_back(padLeft(name, wQ));
            separator.push_back(repeatChar('-', wQ));
        }
        out << joinCols(header) << "\n" << joinCols(separator) << "\n";

        int lastIt = rows.empty() ? 0 : rows.back().it;
        for (auto& row : rows)
        {
            if (!shouldShowIter(row.it, lastIt)) continue;

            std::vector<std::string> line = {
                padLeft(std::to_string(row.it), wIter),
                formatCost(row.cost, wCost)
            };
            auto r = row.Q.getFirstColumnManipulator();
            for (td::UINT4 j = 0; j < pvNames.size(); ++j)
                line.push_back(formatDouble(r(j), 1, wQ));
            out << joinCols(line) << "\n";
        }

        if (convergedAt >= 0)
            out << "\n" << gui::tr("logConverged").c_str() << " " << convergedAt << ".\n";
        out << "\n";
    }

    struct GDResult
    {
        std::string log;
        std::vector<std::string> pvNames; // column order for qHistory
        dense::DblMatrix qHistory;        // (nIterationsRun+1) x pvNames.size(), row = iteration
    };

    // Vanilla gradient descent Volt-VAR controller.
    // All solver parameters come from the UI.
    GDResult runVanillaGD(
        const std::vector<std::string>& pvNames,
        int    maxIter,
        double maxStep,
        double lr      = 5e6,
        double epsilon = 1.0,
        double qMax    = 1800.0,
        double tol     = 1e-3)
    {
        GDResult result;
        std::ostringstream out;

        td::UINT4 nPV = static_cast<td::UINT4>(pvNames.size());

        // Q(k), one row per PV system, initialised to zero.
        dense::DblMatrix Q(nPV, 1, nullptr, true);
        for (auto& name : pvNames)
            setPVkvar(name, 0.0);

        // Initial solve
        Solution_Solve();
        auto voltages = getNodeVoltages();
        double initialCost = voltageCost(voltages);

        appendVoltageTable(out, gui::tr("logInitialVoltages").c_str(), groupVoltagesByBus(voltages));

        std::vector<IterRow> iterRows;
        iterRows.push_back({0, initialCost, Q.makeCopy()});

        VoltageData lastVoltages = voltages;
        int convergedAt = -1;

        for (int it = 1; it <= maxIter; it++)
        {
            dense::DblMatrix grad(nPV, 1);
            auto gW = grad.getFirstColumnManipulator();
            auto qIO = Q.getFirstColumnManipulator();

            for (td::UINT4 j = 0; j < nPV; j++)
            {
                const std::string& name = pvNames[j];
                double baseQ = qIO(j);

                setPVkvar(name, baseQ + epsilon);
                Solution_Solve();
                double cPlus = voltageCost(getNodeVoltages());

                setPVkvar(name, baseQ - epsilon);
                Solution_Solve();
                double cMinus = voltageCost(getNodeVoltages());

                setPVkvar(name, baseQ);
                gW(j) = (cPlus - cMinus) / (2.0 * epsilon);
            }

            double maxChange = 0.0;
            for (td::UINT4 j = 0; j < nPV; j++)
            {
                double step = -lr * gW(j);
                step = std::max(-maxStep, std::min(maxStep, step));
                double newQ = qIO(j) + step;
                newQ = std::max(-qMax, std::min(qMax, newQ));
                maxChange = std::max(maxChange, std::abs(newQ - qIO(j)));
                qIO(j) = newQ;
                setPVkvar(pvNames[j], newQ);
            }

            Solution_Solve();
            lastVoltages = getNodeVoltages();
            double c = voltageCost(lastVoltages);

            iterRows.push_back({it, c, Q.makeCopy()});

            if (maxChange < tol)
            {
                convergedAt = it;
                break;
            }
        }

        appendIterationTable(out, pvNames, iterRows, convergedAt);

        appendVoltageTable(out, gui::tr("logFinalVoltages").c_str(), groupVoltagesByBus(lastVoltages));

        std::string err = lastError();
        if (!err.empty()) out << "\n" << gui::tr("logDssError").c_str() << " " << err << "\n";

        // Assemble the Q-history matrix (rows = iterations run, cols = PV systems) from the snapshots.
        td::UINT4 nRows = static_cast<td::UINT4>(iterRows.size());
        dense::DblMatrix qHistory(nRows, nPV);
        {
            auto w = qHistory.getManipulator();
            for (td::UINT4 r = 0; r < nRows; r++)
            {
                auto qr = iterRows[r].Q.getFirstColumnManipulator();
                for (td::UINT4 j = 0; j < nPV; j++)
                    w(r, j) = qr(j);
            }
        }

        result.log = out.str();
        result.pvNames = pvNames;
        result.qHistory = qHistory;
        return result;
    }
};
