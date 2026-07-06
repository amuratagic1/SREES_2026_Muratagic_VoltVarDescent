#pragma once

struct ExampleScript
{
    const char* titleKey;
    const char* script;
};

inline constexpr int exampleScriptCount()
{
    return 3;
}

inline const ExampleScript& exampleScript(int index)
{
    static const ExampleScript examples[] = {
        {
            "exSimple",
            "Clear\n"
            "\n"
            "Set DefaultBaseFrequency=50\n"
            "\n"
            "New Circuit.VVC_01_Simple_1PV basekv=12.47 pu=1.0 phases=3 bus1=sourcebus\n"
            "\n"
            "New Line.L1 bus1=sourcebus.1.2.3 bus2=bus1.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L2 bus1=bus1.1.2.3      bus2=bus2.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L3 bus1=bus2.1.2.3      bus2=bus3.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "\n"
            "New Load.Load1 bus1=bus1.1.2.3 phases=3 kV=12.47 kW=150 kvar=75  model=1\n"
            "New Load.Load2 bus1=bus2.1.2.3 phases=3 kV=12.47 kW=200 kvar=100 model=1\n"
            "New Load.Load3 bus1=bus3.1.2.3 phases=3 kV=12.47 kW=150 kvar=75  model=1\n"
            "\n"
            "New PVSystem.PV1 bus1=bus3.1.2.3 phases=3 kV=12.47 kVA=1500 Pmpp=1200 irradiance=1 kvar=0\n"
            "\n"
            "Set voltagebases=[12.47]\n"
            "CalcVoltageBases\n"
            "\n"
            "Solve\n"
        },
        {
            "exMedium",
            "Clear\n"
            "\n"
            "Set DefaultBaseFrequency=50\n"
            "\n"
            "New Circuit.VVC_02_Medium_2PV basekv=12.47 pu=1.0 phases=3 bus1=sourcebus\n"
            "\n"
            "New Line.L1 bus1=sourcebus.1.2.3 bus2=bus1.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L2 bus1=bus1.1.2.3      bus2=bus2.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L3 bus1=bus2.1.2.3      bus2=bus3.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L4 bus1=bus3.1.2.3      bus2=bus4.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L5 bus1=bus4.1.2.3      bus2=bus5.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "\n"
            "New Load.Load1 bus1=bus1.1.2.3 phases=3 kV=12.47 kW=200 kvar=100 model=1\n"
            "New Load.Load2 bus1=bus2.1.2.3 phases=3 kV=12.47 kW=300 kvar=150 model=1\n"
            "New Load.Load3 bus1=bus3.1.2.3 phases=3 kV=12.47 kW=250 kvar=125 model=1\n"
            "New Load.Load4 bus1=bus4.1.2.3 phases=3 kV=12.47 kW=300 kvar=150 model=1\n"
            "New Load.Load5 bus1=bus5.1.2.3 phases=3 kV=12.47 kW=200 kvar=100 model=1\n"
            "\n"
            "New PVSystem.PV1 bus1=bus3.1.2.3 phases=3 kV=12.47 kVA=3000 Pmpp=2400 irradiance=1 kvar=0\n"
            "New PVSystem.PV2 bus1=bus5.1.2.3 phases=3 kV=12.47 kVA=3000 Pmpp=2400 irradiance=1 kvar=0\n"
            "\n"
            "Set voltagebases=[12.47]\n"
            "CalcVoltageBases\n"
            "\n"
            "Solve\n"
        },
        {
            "exComplex",
            "Clear\n"
            "Set DefaultBaseFrequency=50\n"
            "\n"
            "New Circuit.VVC_03_Complex_3PV basekv=12.47 pu=1.0 phases=3 bus1=sourcebus\n"
            "\n"
            "New Line.L1 bus1=sourcebus.1.2.3 bus2=bus1.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L2 bus1=bus1.1.2.3      bus2=bus2.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L3 bus1=bus2.1.2.3      bus2=bus3.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L4 bus1=bus3.1.2.3      bus2=bus4.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L5 bus1=bus4.1.2.3      bus2=bus5.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L6 bus1=bus5.1.2.3      bus2=bus6.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "New Line.L7 bus1=bus6.1.2.3      bus2=bus7.1.2.3 phases=3 length=2 units=km r1=0.5 x1=0.8 r0=1.0 x0=2.4 c1=0 c0=0\n"
            "\n"
            "New Load.Load1 bus1=bus1.1.2.3 phases=3 kV=12.47 kW=200 kvar=100 model=1\n"
            "New Load.Load2 bus1=bus2.1.2.3 phases=3 kV=12.47 kW=250 kvar=125 model=1\n"
            "New Load.Load3 bus1=bus3.1.2.3 phases=3 kV=12.47 kW=300 kvar=150 model=1\n"
            "New Load.Load4 bus1=bus4.1.2.3 phases=3 kV=12.47 kW=250 kvar=125 model=1\n"
            "New Load.Load5 bus1=bus5.1.2.3 phases=3 kV=12.47 kW=300 kvar=150 model=1\n"
            "New Load.Load6 bus1=bus6.1.2.3 phases=3 kV=12.47 kW=350 kvar=175 model=1\n"
            "New Load.Load7 bus1=bus7.1.2.3 phases=3 kV=12.47 kW=250 kvar=125 model=1\n"
            "\n"
            "New PVSystem.PV1 bus1=bus3.1.2.3 phases=3 kV=12.47 kVA=2500 Pmpp=2000 irradiance=1 kvar=0\n"
            "New PVSystem.PV2 bus1=bus5.1.2.3 phases=3 kV=12.47 kVA=2500 Pmpp=2000 irradiance=1 kvar=0\n"
            "New PVSystem.PV3 bus1=bus7.1.2.3 phases=3 kV=12.47 kVA=2500 Pmpp=2000 irradiance=1 kvar=0\n"
            "\n"
            "Set voltagebases=[12.47]\n"
            "CalcVoltageBases\n"
            "\n"
            "Solve\n"
        }
    };

    if (index < 0 || index >= exampleScriptCount())
        return examples[0];
    return examples[index];
}
