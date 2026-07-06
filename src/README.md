# Volt-VAR Descent

**Volt-VAR Descent** is a cross-platform desktop application for interactive **Volt-VAR control (VVC)** on electric distribution feeders. It optimizes the reactive power (Q) setpoints of PV systems to flatten the network voltage profile, using **gradient descent** driven by repeated OpenDSS power-flow solutions.

The application is built on the [natID](https://github.com/idzafic/natID) GUI framework and uses [`dss_capi`](https://github.com/dss-extensions/dss_capi) (the OpenDSS C API) as its power-flow engine.

---

## Features

- **Two-tab workflow** — *Network* for model editing and validation, *Solver* for optimization and results.
- **Built-in DSS editor** with line numbering, plus load/save and example feeders.
- **One-click validation** that parses the circuit and reports a live network summary (buses, nodes, lines, loads, transformers, capacitors, generators, PV systems).
- **Configurable solver** — max iterations, learning rate, max Q step, gradient step ($\varepsilon$), Q limit, and convergence tolerance are all editable from the UI.
- **Dual result views** — a formatted text report (initial/final voltage tables and a per-iteration cost/Q trace) and an interactive plot of each PV system's Q over the iterations.
- **Bundled example feeders** with 1, 2, and 3 PV systems.
- **Cross-platform** — macOS (Intel & Apple Silicon) and Windows.

---

## How It Works

1. **Load a network** on the *Network* tab: start from scratch, open a `.dss` file, or pick a built-in example.
2. **Validate** the script. The circuit is compiled through `dss_capi`; on success the network summary is populated and the *Solver* tab fully funcitonal. Editing the script again invalidates it and re-locks the solver.
3. **Configure and run** on the *Solver* tab. The solver initializes all PV systems to `Q = 0`, then iteratively:
   - estimates the cost gradient for each PV system via a central difference,
   - takes a bounded, learning-rate-scaled descent step,
   - re-solves the circuit and records the new cost,
   - stops when the largest Q change falls below the convergence tolerance or the iteration limit is reached.
4. **Inspect results** as a text report or as a Q-vs-iteration plot.

---

## Project Structure

```
VoltVarDescent/
├── CMakeLists.txt            Top-level CMake entry point
├── voltVarDescent.cmake      Build rules, dss_capi linking, dylib bundling
├── examples/                 Sample OpenDSS feeders (1/2/3 PV systems)
├── res/                      Icons, app icon, and EN/BA translations
└── src/
    ├── main.cpp              Entry point
    ├── Application.h         natID application bootstrap
    ├── MainWindow.h          Top-level window
    ├── MainView.h            Tab container (Network / Solver), owns DSSEngine
    ├── DSSEngine.h           dss_capi wrapper + gradient-descent solver
    ├── network/              Network tab (welcome, editor, summary, examples)
    └── solver/               Solver tab (controls, text output, plot)
```

---

## Building

### Prerequisites

- **CMake** 3.18 or newer and a C++ toolchain (Xcode on macOS, Visual Studio on Windows).
- **natID SDK** installed at `~/natID.SDK` (provides `natGUI` and `natPlot`).
- **dss_capi** installed at `~/dss_capi` (macOS) or `~/dss_capi/win_x64` (Windows), with the shared libraries under the platform `lib/` directory.

### Configure and build

```bash
mkdir xbuild && cd xbuild
cmake ..
cmake --build . --config Release
```

---

## Usage Notes

- The solver requires at least one `PVSystem` in the circuit; otherwise it reports that no PV systems were found.
- The UI ships with English (`EN`) and Bosnian (`BA`) translations; the active language is set in `main.cpp` via `app.init(...)`.

---

## Future Improvements

- **Additional gradient descent algorithms** — the algorithm selector already lists *Adaptive GD* and *Momentum*; these are placeholders and only *Vanilla GD* is currently implemented.
- **Selectable cost function** — expose the cost-function dropdown (*Power Loss*, *Voltage Limit Violation*, *Reactive Power Penalty*)
- **Step-through execution** — allow the user to advance the gradient-descent algorithm one iteration at a time to inspect intermediate voltages, gradients, ....

---

## Known Bugs

- **Validate button reset** — the Validate button does not always reset to its unvalidated state correctly.
- **Error message overflow** — a long validation error message can expand the Validate button beyond its intended size.
- **Plot label clipping** — function (series) names on the solver plot can render out of bounds and require a window resize before they display correctly.
