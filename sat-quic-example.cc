/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Magister Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Budiarto Herman <budiarto.herman@magister.fi>
 *
 */

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/satellite-module.h"
#include "ns3/traffic-module.h"

using namespace ns3;

/**
 * \file sat-QUIC-example.cc
 * \ingroup satellite
 * \brief Example of using QUIC traffic model in a satellite network.
 *
 * One QUIC server application is installed in the first GW user. Then one QUIC
 * client application is installed in each UT user, configured to point to the
 * server. TCP protocol is used between the applications.
 *
 * By default, the SIMPLE test scenario is used. Another test scenario can be
 * given from command line as user argument, e.g.:
 *
 *     $ ./waf --run="sat-QUIC-example --scenario=larger"
 *     $ ./waf --run="sat-QUIC-example --scenario=full"
 *
 * Simulation runs for 1000 seconds by default. This can be changed from the
 * command line argument as well, e.g.:
 *
 *     $ ./waf --run="sat-QUIC-example --duration=500"
 *
 * To see help for user arguments:
 *
 *     $ ./waf --run "sat-QUIC-example --PrintHelp"
 *
 */
NS_LOG_COMPONENT_DEFINE("sat-QUIC-example");

int
main(int argc, char* argv[])
{
    std::string scenario = "simple";
    double duration = 1000;
    SatHelper::PreDefinedScenario_t satScenario = SatHelper::SIMPLE;

    Config::SetDefault("ns3::SatHelper::ScenarioCreationTraceEnabled", BooleanValue(true));

    auto simulationHelper = CreateObject<SimulationHelper>("example-QUIC");
    Config::SetDefault("ns3::SatEnvVariables::EnableSimulationOutputOverwrite", BooleanValue(true));

    // read command line parameters given by user
    CommandLine cmd;
    cmd.AddValue("scenario", "Test scenario to use. (simple, larger or full)", scenario);
    cmd.AddValue("duration", "Simulation duration (in seconds)", duration);
    simulationHelper->AddDefaultUiArguments(cmd);
    cmd.Parse(argc, argv);

    if (scenario == "larger")
    {
        satScenario = SatHelper::LARGER;
    }
    else if (scenario == "full")
    {
        satScenario = SatHelper::FULL;
    }

    /// Set simulation output details
    simulationHelper->SetSimulationTime(duration);
    simulationHelper->SetOutputTag(scenario);

    // LogComponentEnableAll (LOG_PREFIX_ALL);
    // LogComponentEnable ("QUICClient", LOG_LEVEL_ALL);
    // LogComponentEnable ("QUICServer", LOG_LEVEL_ALL);
    LogComponentEnable("sat-QUIC-example", LOG_LEVEL_INFO);

    // remove next line from comments to run real time simulation
    // GlobalValue::Bind ("SimulatorImplementationType", StringValue
    // ("ns3::RealtimeSimulatorImpl"));

    // Creating the reference system. Note, currently the satellite module supports
    // only one reference system, which is named as "Scenario72". The string is utilized
    // in mapping the scenario to the needed reference system configuration files. Arbitrary
    // scenario name results in fatal error.
    Ptr<SatHelper> helper = simulationHelper->CreateSatScenario(satScenario);

    // get users
    NodeContainer utUsers = helper->GetUtUsers();
    NodeContainer gwUsers = helper->GetGwUsers();

    QuicHelper stack;
    stack.InstallQuic (gwUsers);
    uint32_t dlPort = 1025;
    QuicServerHelper dlPacketSinkHelper (dlPort);
    NetDeviceContainer devices;
    devices = pointToPoint.Install (gwUsers);

    InternetStackHelper stack;
    stack.Install (gwUsers);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign (devices);
    
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;


    uint32_t dlPort = 1025;
    QuicServerHelper dlPacketSinkHelper (dlPort);
    serverApps.Add (dlPacketSinkHelper.Install (gwUsers.Get(0)));

    double interPacketInterval = 1000;
    QuicClientHelper dlClient (interfaces.GetAddress (1), dlPort);
    dlClient.SetAttribute ("Interval", TimeValue (MicroSeconds(interPacketInterval)));
    dlClient.SetAttribute ("PacketSize", UintegerValue(1000));
    dlClient.SetAttribute ("MaxPackets", UintegerValue(10000000));
    clientApps.Add (dlClient.Install (gwUsers.Get(1)));
    
    uint32_t i = 0;
    std::vector<Ptr<ClientRxTracePlot>> plots;
    for (auto app = clientApps.Begin(); app != clientApps.End(); app++, i++)
    {
        std::stringstream plotName;
        plotName << "3GPP-QUIC-client-" << i << "-trace";
        plots.push_back(CreateObject<ClientRxTracePlot>(*app, plotName.str()));
    }

    NS_LOG_INFO("--- sat-QUIC-example ---");
    NS_LOG_INFO("  Scenario used: " << scenario);
    NS_LOG_INFO("  ");

    simulationHelper->EnableProgressLogs();

    // Add PLT statistics
    Ptr<SatStatsHelperContainer> s = simulationHelper->GetStatisticsContainer();
    s->AddGlobalFwdAppPlt(SatStatsHelper::OUTPUT_SCALAR_FILE);
    s->AddGlobalFwdAppPlt(SatStatsHelper::OUTPUT_SCATTER_FILE);
    s->AddPerUtFwdAppPlt(SatStatsHelper::OUTPUT_SCALAR_FILE);
    s->AddPerUtFwdAppPlt(SatStatsHelper::OUTPUT_SCATTER_FILE);
    s->AddPerGwFwdAppPlt(SatStatsHelper::OUTPUT_SCALAR_FILE);
    s->AddPerGwFwdAppPlt(SatStatsHelper::OUTPUT_SCATTER_FILE);

    simulationHelper->RunSimulation();

    plots.clear();

    return 0;

} // end of `int main (int argc, char *argv[])`
