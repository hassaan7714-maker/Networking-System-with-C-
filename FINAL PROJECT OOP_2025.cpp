#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <cctype>
#include <limits>
#include <conio.h>
using namespace std;
// ========== Console Utilities ==========
class Console {
public:
    enum Color { BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE,
                GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN,
                LIGHT_RED, LIGHT_MAGENTA, LIGHT_YELLOW, BRIGHT_WHITE };

    static void setColor(Color fg, Color bg = BLACK) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, fg | (bg << 4));
    }
    static void reset() { setColor(WHITE, BLACK); }
    static void clear() { system("cls"); }
    static void sleep(int ms) { Sleep(ms); }
    static void drawHeader(const string& title) {
        setColor(LIGHT_CYAN);
        cout << "\n" << string(50, '=') << "\n  " << title << "\n" << string(50, '=') << "\n";
        reset();
    }
    static void drawSubHeader(const string& title) {
        setColor(LIGHT_BLUE);
        cout << "\n" << string(40, '-') << "\n  " << title << "\n" << string(40, '-') << "\n";
        reset();
    }
    static void drawProgressBar(int percent) {
        setColor(LIGHT_GREEN);
        cout << "[";
        for (int i = 0; i < 20; i++) cout << (i < percent/5 ? "=" : " ");
        cout << "] " << percent << "%\r";
        cout.flush();
        reset();
    }
    static void drawMenuBox(const vector<string>& options) {
        setColor(LIGHT_MAGENTA);
        cout << "\n" << string(30, '=') << "\n";
        for (size_t i = 0; i < options.size(); i++) {
            cout << " " << i+1 << ". " << options[i] << "\n";
        }
        cout << string(30, '=') << "\n";
        reset();
    }
    
    static void showError(const string& message) {
        setColor(LIGHT_RED);
        cout << "[!] " << message << endl;
        reset();
    }
    
    static void showSuccess(const string& message) {
        setColor(LIGHT_GREEN);
        cout << "[+] " << message << endl;
        reset();
    }
    
    static void showWarning(const string& message) {
        setColor(YELLOW);
        cout << "[!] " << message << endl;
        reset();
    }
    
    static void showInfo(const string& message) {
        setColor(LIGHT_BLUE);
        cout << "[i] " << message << endl;
        reset();
    }
    
    static void drawNetworkTopology(const vector<string>& devices) {
        setColor(LIGHT_CYAN);
        cout << "\nNETWORK TOPOLOGY:\n";
        cout << "┌───────────────────────────────────────┐\n";
        cout << "│                                       │\n";
        
        for (size_t i = 0; i < devices.size(); i++) {
            cout << "│  ○ " << left << setw(35) << devices[i] << " │\n";
            if (i < devices.size() - 1) {
                cout << "│  │" << string(36, ' ') << "│\n";
                cout << "│  │" << string(36, ' ') << "│\n";
                cout << "│  ↓" << string(36, ' ') << "│\n";
            }
        }
        
        cout << "│                                       │\n";
        cout << "└───────────────────────────────────────┘\n";
        reset();
    }
};

// ========== Logger System ==========
class Logger {
    static const string LOG_FILE;
    
public:
    static void log(const string& message) {
        ofstream logFile(LOG_FILE, ios::app);
        if (logFile.is_open()) {
            time_t now = time(nullptr);
            char timeStr[20];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
            
            logFile << "[" << timeStr << "] " << message << endl;
            logFile.close();
        }
    }
    
    static void clearLog() {
        ofstream logFile(LOG_FILE);
        if (logFile.is_open()) {
            logFile << "=== NETWORK SIMULATOR LOG ===\n";
            logFile.close();
        }
    }
    
    static void viewLog() {
        ifstream logFile(LOG_FILE);
        if (logFile.is_open()) {
            Console::drawHeader("SYSTEM LOG");
            string line;
            while (getline(logFile, line)) {
                if (line.find("ERROR") != string::npos) {
                    Console::setColor(Console::LIGHT_RED);
                } else if (line.find("WARNING") != string::npos) {
                    Console::setColor(Console::YELLOW);
                } else if (line.find("INFO") != string::npos) {
                    Console::setColor(Console::LIGHT_BLUE);
                } else {
                    Console::setColor(Console::WHITE);
                }
                cout << line << endl;
            }
            Console::reset();
            logFile.close();
        } else {
            Console::showError("Unable to open log file");
        }
    }
};
const string Logger::LOG_FILE = "network_simulator.log";

// Forward declarations
class NetworkDevice;
class Router;
class Switch;
class Firewall;
class NetworkSimulator;

// ========== Help System ==========
class HelpSystem {
public:
    static void showGeneralHelp() {
        Console::drawHeader("HELP SYSTEM");
        Console::setColor(Console::LIGHT_CYAN);
        cout << "This Enterprise Network Simulator allows you to:\n";
        cout << "1. Create and manage network devices (Routers, Switches, Firewalls)\n";
        cout << "2. Simulate network traffic and operations\n";
        cout << "3. Run diagnostics and view device status\n";
        cout << "4. Save and load network configurations\n";
        cout << "5. View network topology and statistics\n\n";
        
        Console::setColor(Console::YELLOW);
        cout << "Navigation:\n";
        cout << "- Use numbers to select menu options\n";
        cout << "- Press Enter to continue after each operation\n";
        cout << "- Admin functions require password (default: 'admin123')\n";
        Console::reset();
    }
    
    static void showDeviceHelp(const string& deviceType) {
        Console::drawHeader(deviceType + " HELP");
        if (deviceType == "Router") {
            Console::setColor(Console::LIGHT_BLUE);
            cout << "Routers are networking devices that forward data packets between networks.\n";
            cout << "Key Features:\n";
            cout << "- Routing tables\n";
            cout << "- Packet forwarding\n";
            cout << "- Basic firewall capabilities\n";
            cout << "- Network address translation (NAT)\n";
        } else if (deviceType == "Switch") {
            Console::setColor(Console::LIGHT_MAGENTA);
            cout << "Switches are networking devices that connect devices on a network.\n";
            cout << "Key Features:\n";
            cout << "- Multiple ports for device connections\n";
            cout << "- MAC address learning\n";
            cout << "- VLAN support\n";
            cout << "- Spanning Tree Protocol (STP)\n";
        } else if (deviceType == "Firewall") {
            Console::setColor(Console::LIGHT_RED);
            cout << "Firewalls are security devices that monitor and control network traffic.\n";
            cout << "Key Features:\n";
            cout << "- Access control lists (ACLs)\n";
            cout << "- Intrusion detection/prevention\n";
            cout << "- VPN support\n";
            cout << "- Threat monitoring\n";
        }
        Console::reset();
    }
    
    static void showAdminHelp() {
        Console::drawHeader("ADMIN HELP");
        Console::setColor(Console::LIGHT_GREEN);
        cout << "Admin functions allow you to:\n";
        cout << "- View system logs\n";
        cout <<"- Clear logs\n";
        cout << "- Backup/restore configurations\n";
        cout << "- Reset the simulator\n";
        cout << "- Change admin password\n\n";
        cout << "Default admin password: admin123\n";
        Console::reset();
    }
};

// ========== Base Network Device (updated) ==========
class NetworkDevice {
protected:
    string id;
    string name;
    string manufacturer;
    string model;
    bool isPoweredOn;
    time_t powerOnTime;
    static int totalDevices;
    static int activeDevices;

    string generateID() {
        const string chars = "ABCDEF0123456789";
        string id = "DEV-";
        for (int i = 0; i < 6; i++) id += chars[rand() % chars.length()];
        return id;
    }

public:
    NetworkDevice(string name, string mfr, string mdl) 
        : name(name), manufacturer(mfr), model(mdl), isPoweredOn(false) {
        id = generateID();
        totalDevices++;
        Logger::log("INFO: Created device " + id + " (" + name + ")");
    }

    virtual ~NetworkDevice() { 
        if (isPoweredOn) activeDevices--;
        totalDevices--; 
        Logger::log("INFO: Destroyed device " + id + " (" + name + ")");
    }

    virtual void powerOn() {
        if (!isPoweredOn) {
            isPoweredOn = true;
            powerOnTime = time(nullptr);
            activeDevices++;
            Console::setColor(Console::LIGHT_GREEN);
            cout << "[+] " << name << " (" << id << ") powered ON\n";
            Console::reset();
            Logger::log("INFO: Device " + id + " powered ON");
        }
    }

    virtual void powerOff() {
        if (isPoweredOn) {
            isPoweredOn = false;
            activeDevices--;
            Console::setColor(Console::LIGHT_RED);
            cout << "[-] " << name << " (" << id << ") powered OFF\n";
            Console::reset();
            Logger::log("INFO: Device " + id + " powered OFF");
        }
    }

    virtual void displayInfo() const = 0;
    virtual void runDiagnostics() = 0;
    virtual string getType() const = 0;
    
    virtual void runDiagnostics(bool detailed) {
        if (detailed) {
            Console::setColor(Console::YELLOW);
            cout << "\nRunning detailed diagnostics on " << name << "...\n";
            Console::sleep(1000);
            cout << "1. Advanced hardware check: OK\n";
            Console::sleep(500);
            cout << "2. Performance metrics: NORMAL\n";
            Console::sleep(300);
            cout << "3. Security audit: PASSED\n";
            Console::setColor(Console::LIGHT_GREEN);
            cout << "All detailed diagnostics passed!\n";
            Console::reset();
            Logger::log("INFO: Detailed diagnostics passed for " + id);
        } else {
            runDiagnostics();
        }
    }

    string getUptime() const {
        if (!isPoweredOn) return "00:00:00";
        time_t now = time(nullptr);
        int secs = difftime(now, powerOnTime);
        int hours = secs / 3600;
        int mins = (secs % 3600) / 60;
        int remainingSecs = secs % 60;
        char buffer[9];
        sprintf(buffer, "%02d:%02d:%02d", hours, mins, remainingSecs);
        return string(buffer);
    }

    static int getTotalDevices() { return totalDevices; }
    static int getActiveDevices() { return activeDevices; }
    string getID() const { return id; }
    string getName() const { return name; }
    string getManufacturer() const { return manufacturer; }
    string getModel() const { return model; }
    bool getPowerStatus() const { return isPoweredOn; }
    time_t getPowerOnTime() const { return powerOnTime; }
    
    virtual string toFileString() const {
        return getType() + "," + id + "," + name + "," + manufacturer + "," + model + "," + 
               (isPoweredOn ? "1" : "0") + "," + to_string(powerOnTime);
    }
    
    virtual void fromFileString(const string& data) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(data);
        
        while (getline(tokenStream, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 6) {
            id = tokens[1];
            name = tokens[2];
            manufacturer = tokens[3];
            model = tokens[4];
            isPoweredOn = (tokens[5] == "1");
            if (tokens.size() > 6) {
                powerOnTime = stol(tokens[6]);
            }
        }
    }
    
    friend void displayNetworkSummary(const NetworkDevice& device);
    friend class NetworkSimulator;
};
int NetworkDevice::totalDevices = 0;
int NetworkDevice::activeDevices = 0;

void displayNetworkSummary(const NetworkDevice& device) {
    Console::setColor(Console::LIGHT_CYAN);
    cout << "\n=== NETWORK DEVICE SUMMARY ===" << endl;
    Console::setColor(Console::WHITE);
    cout << "Device Name: " << device.name << endl;
    cout << "ID: " << device.id << endl;
    cout << "Type: " << device.getType() << endl;
    cout << "Status: " << (device.isPoweredOn ? "ONLINE" : "OFFLINE") << endl;
    if (device.isPoweredOn) {
        cout << "Uptime: " << device.getUptime() << endl;
    }
    cout << "Total Devices in Network: " << NetworkDevice::totalDevices << endl;
    cout << "Active Devices: " << NetworkDevice::activeDevices << endl;
    Console::reset();
}

// ========== Enterprise Router (updated) ==========
class Router : public NetworkDevice {
    vector<string> connectedDevices;
    map<string, string> routingTable;
    string firmware;
    bool firewallEnabled;
    int packetsForwarded;
    static int totalRouters;

public:
    Router(string name, string mfr = "Cisco", string mdl = "ISR-4000", string fw = "IOS-XE-17.06")
        : NetworkDevice(name, mfr, mdl), firmware(fw), firewallEnabled(true), packetsForwarded(0) {
        totalRouters++;
        Logger::log("INFO: Created router " + id);
    }
    int getRoutingTableSize() const { return routingTable.size(); }

    ~Router() {
        totalRouters--;
        Logger::log("INFO: Destroyed router " + id);
    }

    void addRoute(string network, string gateway) {
        routingTable[network] = gateway;
        Console::setColor(Console::YELLOW);
        cout << "[ROUTE] Added route to " << network << " via " << gateway << endl;
        Console::reset();
        Logger::log("INFO: Added route to " + network + " on router " + id);
    }

    void connectDevice(string devID) {
        if (find(connectedDevices.begin(), connectedDevices.end(), devID) == connectedDevices.end()) {
            connectedDevices.push_back(devID);
            Logger::log("INFO: Connected device " + devID + " to router " + id);
        }
    }

    void forwardPacket() { 
        packetsForwarded++; 
        Logger::log("INFO: Router " + id + " forwarded packet");
    }

    void forwardPacket(int count) {
        packetsForwarded += count;
        Console::setColor(Console::LIGHT_BLUE);
        cout << "[ROUTER] " << name << " forwarded " << count << " packets (Total: " << packetsForwarded << ")\n";
        Console::reset();
        Logger::log("INFO: Router " + id + " forwarded " + to_string(count) + " packets");
    }

    void displayInfo() const override {
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\n[ROUTER] " << name << " (" << id << ")\n";
        Console::setColor(Console::WHITE);
        cout << "|- Manufacturer: " << manufacturer << endl;
        cout << "|- Model: " << model << endl;
        cout << "|- Firmware: " << firmware << endl;
        cout << "|- Status: " << (isPoweredOn ? "ONLINE" : "OFFLINE") << endl;
        cout << "|- Uptime: " << getUptime() << endl;
        cout << "|- Firewall: " << (firewallEnabled ? "ENABLED" : "DISABLED") << endl;
        cout << "|- Routes: " << routingTable.size() << endl;
        cout << "|- Connected devices: " << connectedDevices.size() << endl;
        cout << "`- Packets forwarded: " << packetsForwarded << endl;
        Console::reset();
    }

    void runDiagnostics() override {
        Console::setColor(Console::YELLOW);
        cout << "\nRunning diagnostics on " << name << "...\n";
        Console::sleep(500);
        cout << "1. Hardware check: OK\n";
        Console::sleep(300);
        cout << "2. Routing table: " << routingTable.size() << " valid routes\n";
        Console::sleep(200);
        cout << "3. Interfaces: " << connectedDevices.size() << " active\n";
        Console::setColor(Console::LIGHT_GREEN);
        cout << "All diagnostics passed!\n";
        Console::reset();
        Logger::log("INFO: Diagnostics passed for router " + id);
    }

    string getType() const override { return "Router"; }
    string getFirmware() const { return firmware; }
    bool getFirewallStatus() const { return firewallEnabled; }
    int getPacketsForwarded() const { return packetsForwarded; }
    
    static int getTotalRouters() { return totalRouters; }
    
    string toFileString() const override {
        string base = NetworkDevice::toFileString();
        base += "," + firmware + "," + (firewallEnabled ? "1" : "0") + "," + to_string(packetsForwarded);
        
        // Save routing table
        base += "," + to_string(routingTable.size());
        for (const auto& route : routingTable) {
            base += "," + route.first + "," + route.second;
        }
        
        // Save connected devices
        base += "," + to_string(connectedDevices.size());
        for (const auto& dev : connectedDevices) {
            base += "," + dev;
        }
        
        return base;
    }
    
    void fromFileString(const string& data) override {
        vector<string> tokens;
        string token;
        istringstream tokenStream(data);
        
        while (getline(tokenStream, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 9) {
            NetworkDevice::fromFileString(data);
            firmware = tokens[7];
            firewallEnabled = (tokens[8] == "1");
            packetsForwarded = stoi(tokens[9]);
            
            // Load routing table
            int routeCount = stoi(tokens[10]);
            routingTable.clear();
            for (int i = 0; i < routeCount; i++) {
                if (11 + i*2 < tokens.size()) {
                    routingTable[tokens[11 + i*2]] = tokens[12 + i*2];
                }
            }
            
            // Load connected devices
            int devPos = 11 + routeCount*2;
            if (devPos < tokens.size()) {
                int devCount = stoi(tokens[devPos]);
                connectedDevices.clear();
                for (int i = 0; i < devCount; i++) {
                    if (devPos + 1 + i < tokens.size()) {
                        connectedDevices.push_back(tokens[devPos + 1 + i]);
                    }
                }
            }
        }
    }
    
    friend void analyzeRouterPerformance(const Router& router);
};
int Router::totalRouters = 0;

void analyzeRouterPerformance(const Router& router) {
    Console::setColor(Console::LIGHT_MAGENTA);
    cout << "\n=== ROUTER PERFORMANCE ANALYSIS ===" << endl;
    Console::setColor(Console::WHITE);
    cout << "Router Name: " << router.name << endl;
    cout << "Packets Forwarded: " << router.packetsForwarded << endl;
    cout << "Routes Configured: " << router.routingTable.size() << endl;
    cout << "Connected Devices: " << router.connectedDevices.size() << endl;
    
    double efficiency = (router.packetsForwarded > 0) ? 
        (static_cast<double>(router.routingTable.size()) / router.packetsForwarded * 100) : 0;
    cout << "Routing Efficiency: " << fixed << setprecision(2) << efficiency << "%" << endl;
    
    if (efficiency > 75) {
        Console::setColor(Console::LIGHT_GREEN);
        cout << "Performance: EXCELLENT" << endl;
    } else if (efficiency > 50) {
        Console::setColor(Console::YELLOW);
        cout << "Performance: GOOD" << endl;
    } else {
        Console::setColor(Console::LIGHT_RED);
        cout << "Performance: NEEDS IMPROVEMENT" << endl;
    }
    Console::reset();
    
    Logger::log("INFO: Performance analysis for router " + router.id + 
                " - Efficiency: " + to_string(efficiency) + "%");
}

// ========== Layer 3 Switch (updated) ==========
class Switch : public NetworkDevice {
    struct Port {
        string connectedDevice;
        bool isActive;
        int speed;
    };

    vector<Port> ports;
    bool spanningTreeEnabled;
    int framesForwarded;
    static int totalSwitches;

public:
    Switch(string name, int portCount = 48, string mfr = "Juniper", string mdl = "EX4300")
        : NetworkDevice(name, mfr, mdl), spanningTreeEnabled(true), framesForwarded(0) {
        ports.resize(portCount);
        totalSwitches++;
        Logger::log("INFO: Created switch " + id + " with " + to_string(portCount) + " ports");
    }

    ~Switch() {
        totalSwitches--;
        Logger::log("INFO: Destroyed switch " + id);
    }

    void connectDevice(int port, string devID, int speed = 1000) {
        if (port >= 0 && port < ports.size()) {
            ports[port].connectedDevice = devID;
            ports[port].isActive = true;
            ports[port].speed = speed;
            Logger::log("INFO: Connected device " + devID + " to switch " + id + " port " + to_string(port));
        }
    }

    void forwardFrame() { 
        framesForwarded++; 
        Logger::log("INFO: Switch " + id + " forwarded frame");
    }
    
    void forwardFrame(int count) {
        framesForwarded += count;
        Console::setColor(Console::LIGHT_BLUE);
        cout << "[SWITCH] " << name << " forwarded " << count << " frames (Total: " << framesForwarded << ")\n";
        Console::reset();
        Logger::log("INFO: Switch " + id + " forwarded " + to_string(count) + " frames");
    }

    void displayInfo() const override {
        int activePorts = countActivePorts();
        Console::setColor(Console::LIGHT_MAGENTA);
        cout << "\n[SWITCH] " << name << " (" << id << ")\n";
        Console::setColor(Console::WHITE);
        cout << "|- Manufacturer: " << manufacturer << endl;
        cout << "|- Model: " << model << endl;
        cout << "|- Status: " << (isPoweredOn ? "ONLINE" : "OFFLINE") << endl;
        cout << "|- Uptime: " << getUptime() << endl;
        cout << "|- Ports: " << ports.size() << " (" << activePorts << " active)\n";
        cout << "|- STP: " << (spanningTreeEnabled ? "ENABLED" : "DISABLED") << endl;
        cout << "`- Frames forwarded: " << framesForwarded << endl;
        Console::reset();
    }

    void runDiagnostics() override {
        Console::setColor(Console::YELLOW);
        cout << "\nRunning diagnostics on " << name << "...\n";
        Console::sleep(500);
        cout << "1. Backplane test: OK\n";
        Console::sleep(300);
        cout << "2. Port status: " << countActivePorts() << "/" << ports.size() << " active\n";
        Console::sleep(200);
        cout << "3. MAC table: Operational\n";
        Console::setColor(Console::LIGHT_GREEN);
        cout << "All diagnostics passed!\n";
        Console::reset();
        Logger::log("INFO: Diagnostics passed for switch " + id);
    }

    string getType() const override { return "Switch"; }
    bool getSpanningTreeStatus() const { return spanningTreeEnabled; }
    int getFramesForwarded() const { return framesForwarded; }
    int getPortCount() const { return ports.size(); }
    
    static int getTotalSwitches() { return totalSwitches; }
    
    string toFileString() const override {
        string base = NetworkDevice::toFileString();
        base =base + "," + (spanningTreeEnabled ? "1" : "0") + "," + to_string(framesForwarded);
        
        // Save port information
        base = base + "," + to_string(ports.size());
        for (const auto& port : ports) {
            base = base +"," + (port.isActive ? "1" : "0") + "," + to_string(port.speed) + "," + port.connectedDevice;
        }
        
        return base;
    }
    
    void fromFileString(const string& data) override {
        vector<string> tokens;
        string token;
        istringstream tokenStream(data);
        
        while (getline(tokenStream, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 8) {
            NetworkDevice::fromFileString(data);
            spanningTreeEnabled = (tokens[7] == "1");
            framesForwarded = stoi(tokens[8]);
            
            // Load port information
            int portCount = stoi(tokens[9]);
            ports.resize(portCount);
            
            int tokenPos = 10;
            for (int i = 0; i < portCount && tokenPos + 2 < tokens.size(); i++) {
                ports[i].isActive = (tokens[tokenPos] == "1");
                ports[i].speed = stoi(tokens[tokenPos + 1]);
                ports[i].connectedDevice = tokens[tokenPos + 2];
                tokenPos += 3;
            }
        }
    }
    
    friend void checkSwitchPorts(const Switch& sw);

public:
    int countActivePorts() const {
        int count = 0;
        for (const auto& port : ports) if (port.isActive) count++;
        return count;
    }
};
int Switch::totalSwitches = 0;

void checkSwitchPorts(const Switch& sw) {
    Console::setColor(Console::LIGHT_CYAN);
    cout << "\n=== SWITCH PORT ANALYSIS ===" << endl;
    Console::setColor(Console::WHITE);
    cout << "Switch Name: " << sw.name << endl;
    cout << "Total Ports: " << sw.ports.size() << endl;
    cout << "Active Ports: " << sw.countActivePorts() << endl;
    
    int highSpeedPorts = 0;
    for (const auto& port : sw.ports) {
        if (port.isActive && port.speed >= 1000) {
            highSpeedPorts++;
        }
    }
    
    cout << "High-Speed Ports (1Gbps+): " << highSpeedPorts << endl;
    double utilization = (sw.ports.size() > 0) ? 
        (static_cast<double>(sw.countActivePorts()) / sw.ports.size()) * 100 : 0;
    cout << "Port Utilization: " << fixed << setprecision(2) << utilization << "%" << endl;
    
    if (utilization > 80) {
        Console::setColor(Console::LIGHT_RED);
        cout << "Warning: High port utilization!" << endl;
    } else if (utilization > 50) {
        Console::setColor(Console::YELLOW);
        cout << "Notice: Moderate port utilization" << endl;
    } else {
        Console::setColor(Console::LIGHT_GREEN);
        cout << "Status: Healthy port utilization" << endl;
    }
    Console::reset();
    
    Logger::log("INFO: Port analysis for switch " + sw.id + 
                " - Utilization: " + to_string(utilization) + "%");
}

// ========== Enterprise Firewall (updated) ==========
class Firewall : public NetworkDevice {
    map<string, bool> securityRules;
    bool idsEnabled;
    int blockedAttempts;
    static int totalFirewalls;

public:
    Firewall(string name, string mfr = "Palo Alto", string mdl = "PA-5200")
        : NetworkDevice(name, mfr, mdl), idsEnabled(true), blockedAttempts(0) {
        securityRules["HTTP"] = true;
        securityRules["HTTPS"] = true;
        securityRules["SSH"] = true;
        securityRules["FTP"] = false;
        totalFirewalls++;
        Logger::log("INFO: Created firewall " + id);
    }

    ~Firewall() {
        totalFirewalls--;
        Logger::log("INFO: Destroyed firewall " + id);
    }
    int getSecurityRulesCount() const { return securityRules.size(); }

    void addRule(string service, bool allow) {
        securityRules[service] = allow;
        Logger::log("INFO: Added rule for " + service + " on firewall " + id + 
                   " (" + (allow ? "ALLOW" : "DENY") + ")");
    }

    void blockAttack() { 
        blockedAttempts++; 
        Logger::log("INFO: Firewall " + id + " blocked attack");
    }
    
    void blockAttack(int count) {
        blockedAttempts += count;
        Console::setColor(Console::LIGHT_RED);
        cout << "[FIREWALL] " << name << " blocked " << count << " attacks (Total: " << blockedAttempts << ")\n";
        Console::reset();
        Logger::log("INFO: Firewall " + id + " blocked " + to_string(count) + " attacks");
    }

    void displayInfo() const override {
        Console::setColor(Console::LIGHT_RED);
        cout << "\n[FIREWALL] " << name << " (" << id << ")\n";
        Console::setColor(Console::WHITE);
        cout << "|- Manufacturer: " << manufacturer << endl;
        cout << "|- Model: " << model << endl;
        cout << "|- Status: " << (isPoweredOn ? "ONLINE" : "OFFLINE") << endl;
        cout << "|- Uptime: " << getUptime() << endl;
        cout << "|- IDS: " << (idsEnabled ? "ENABLED" : "DISABLED") << endl;
        cout << "|- Security rules: " << securityRules.size() << endl;
        cout << "`- Attacks blocked: " << blockedAttempts << endl;
        Console::reset();
    }

    void runDiagnostics() override {
        Console::setColor(Console::YELLOW);
        cout << "\nRunning diagnostics on " << name << "...\n";
        Console::sleep(500);
        cout << "1. Policy check: OK\n";
        Console::sleep(300);
        cout << "2. Threat detection: ACTIVE\n";
        Console::sleep(200);
        cout << "3. Packet inspection: OPERATIONAL\n";
        Console::setColor(Console::LIGHT_GREEN);
        cout << "All diagnostics passed!\n";
        Console::reset();
        Logger::log("INFO: Diagnostics passed for firewall " + id);
    }

    string getType() const override { return "Firewall"; }
    bool getIdsStatus() const { return idsEnabled; }
    int getBlockedAttempts() const { return blockedAttempts; }
    
    static int getTotalFirewalls() { return totalFirewalls; }
    
    string toFileString() const override {
        string base = NetworkDevice::toFileString();
        base =base + "," + (idsEnabled ? "1" : "0") + "," + to_string(blockedAttempts);
        
        // Save security rules
        base += "," + to_string(securityRules.size());
        for (const auto& rule : securityRules) {
            base += "," + rule.first + "," + (rule.second ? "1" : "0");
        }
        
        return base;
    }
    
    void fromFileString(const string& data) override {
        vector<string> tokens;
        string token;
        istringstream tokenStream(data);
        
        while (getline(tokenStream, token, ',')) {
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 8) {
            NetworkDevice::fromFileString(data);
            idsEnabled = (tokens[7] == "1");
            blockedAttempts = stoi(tokens[8]);
            
            // Load security rules
            int ruleCount = stoi(tokens[9]);
            securityRules.clear();
            for (int i = 0; i < ruleCount; i++) {
                if (10 + i*2 < tokens.size()) {
                    securityRules[tokens[10 + i*2]] = (tokens[11 + i*2] == "1");
                }
            }
        }
    }
};
int Firewall::totalFirewalls = 0;

// ========== Network Simulation System (updated) ==========
class NetworkSimulator {
    vector<NetworkDevice*> devices;
    string adminPassword = "admin123";
    const string CONFIG_FILE = "network_config.cfg";

    // Validation functions
    static bool validateNotEmpty(const string& input) {
        if (input.empty()) {
            Console::showError("This field cannot be empty");
            return false;
        }
        return true;
    }

    static bool validatePositiveNumber(const string& input) {
        if (input.empty()) {
            Console::showError("Please enter a number");
            return false;
        }
        for (char c : input) {
            if (!isdigit(c)) {
                Console::showError("Please enter a valid number");
                return false;
            }
        }
        return true;
    }

    static bool validatePortCount(const string& input) {
        if (!validatePositiveNumber(input)) return false;
        int ports = stoi(input);
        if (ports <= 0 || ports > 128) {
            Console::showError("Port count must be between 1-128");
            return false;
        }
        return true;
    }

    string getValidInput(const string& prompt, const string& defaultValue = "", 
                        bool (*validator)(const string&) = nullptr) {
        string input;
        while (true) {
            cout << prompt;
            if (!defaultValue.empty()) cout << " (" << defaultValue << ")";
            cout << ": ";
            
            getline(cin, input);
            input = input.empty() ? defaultValue : input;
            
            if (!validator || validator(input)) {
                return input;
            }
        }
    }

    void pressEnterToContinue() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
    }
    
    bool authenticateAdmin() {
        string password;
        Console::setColor(Console::LIGHT_MAGENTA);
        cout << "\nEnter admin password: ";
        Console::reset();
        
        // Simple password masking
        char ch;
                while ((ch = getch()) != '\r') { // '\r' is Enter key
            if (ch == '\b') { // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        if (password == adminPassword) {
            Logger::log("INFO: Admin authentication successful");
            return true;
        } else {
            Console::showError("Invalid admin password");
            Logger::log("WARNING: Failed admin authentication attempt");
            return false;
        }
    }

public:
    NetworkSimulator() {
        Logger::clearLog();
        Logger::log("INFO: Network Simulator initialized");
    }

    ~NetworkSimulator() {
        for (auto device : devices) {
            delete device;
        }
        Logger::log("INFO: Network Simulator shutdown");
    }

    void run() {
        while (true) {
            Console::clear();
            Console::drawHeader("ENTERPRISE NETWORK SIMULATOR");
            
            vector<string> mainMenu = {
                "Manage Devices",
                "Simulate Network Operations",
                "Run Diagnostics",
                "View Network Statistics",
                "Admin Functions",
                "Help",
                "Exit"
            };
            
            Console::drawMenuBox(mainMenu);
            
            string choice = getValidInput("Select option (1-7)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: deviceManagementMenu(); break;
                case 2: simulationMenu(); break;
                case 3: diagnosticsMenu(); break;
                case 4: statisticsMenu(); break;
                case 5: adminMenu(); break;
                case 6: helpMenu(); break;
                case 7: return;
                default: Console::showError("Invalid option"); break;
            }
        }
    }

private:
    void deviceManagementMenu() {
        while (true) {
            Console::clear();
            Console::drawHeader("DEVICE MANAGEMENT");
            
            vector<string> menu = {
                "Create New Device",
                "List All Devices",
                "View Device Details",
                "Power On/Off Device",
                "Connect Devices",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-6)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: createDevice(); break;
                case 2: listDevices(); break;
                case 3: viewDeviceDetails(); break;
                case 4: toggleDevicePower(); break;
                case 5: connectDevices(); break;
                case 6: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }

    void createDevice() {
        Console::clear();
        Console::drawHeader("CREATE NEW DEVICE");
        
        vector<string> types = {
            "Router",
            "Switch",
            "Firewall",
            "Cancel"
        };
        
        Console::drawMenuBox(types);
        
        string choice = getValidInput("Select device type (1-4)", "1", validatePositiveNumber);
        int option = stoi(choice);
        
        if (option == 4) return;
        
        string name = getValidInput("Enter device name", "", validateNotEmpty);
        string manufacturer = getValidInput("Enter manufacturer", "Cisco");
        string model = getValidInput("Enter model", "");
        
        NetworkDevice* newDevice = nullptr;
        
        switch (option) {
            case 1: {
                string firmware = getValidInput("Enter firmware version", "IOS-XE-17.06");
                newDevice = new Router(name, manufacturer, model, firmware);
                break;
            }
            case 2: {
                string ports = getValidInput("Enter number of ports", "48", validatePortCount);
                newDevice = new Switch(name, stoi(ports), manufacturer, model);
                break;
            }
            case 3: {
                newDevice = new Firewall(name, manufacturer, model);
                break;
            }
        }
        
        if (newDevice) {
            devices.push_back(newDevice);
            Console::showSuccess("Device created successfully!");
            Logger::log("INFO: Created new " + newDevice->getType() + ": " + name);
        }
    }

    void listDevices() {
        Console::clear();
        Console::drawHeader("DEVICE LIST");
        
        if (devices.empty()) {
            Console::showInfo("No devices in the network");
            return;
        }
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << left << setw(8) << "ID" << setw(15) << "Type" 
             << setw(20) << "Name" << setw(10) << "Status" << endl;
        cout << string(60, '-') << endl;
        
        for (const auto& device : devices) {
            Console::setColor(Console::WHITE);
            cout << left << setw(8) << device->getID().substr(0, 6) 
                 << setw(15) << device->getType()
                 << setw(20) << device->getName();
            
            if (device->getPowerStatus()) {
                Console::setColor(Console::LIGHT_GREEN);
                cout << setw(10) << "ONLINE";
            } else {
                Console::setColor(Console::LIGHT_RED);
                cout << setw(10) << "OFFLINE";
            }
            cout << endl;
        }
        
        Console::reset();
        cout << "\nTotal devices: " << devices.size() << endl;
    }

    void viewDeviceDetails() {
        listDevices();
        if (devices.empty()) return;
        
        string id = getValidInput("\nEnter device ID to view", "", validateNotEmpty);
        
        auto it = find_if(devices.begin(), devices.end(), 
            [&id](NetworkDevice* d) { return d->getID() == id; });
            
        if (it != devices.end()) {
            (*it)->displayInfo();
        } else {
            Console::showError("Device not found");
        }
    }

    void toggleDevicePower() {
        listDevices();
        if (devices.empty()) return;
        
        string id = getValidInput("\nEnter device ID to toggle power", "", validateNotEmpty);
        
        auto it = find_if(devices.begin(), devices.end(), 
            [&id](NetworkDevice* d) { return d->getID() == id; });
            
        if (it != devices.end()) {
            if ((*it)->getPowerStatus()) {
                (*it)->powerOff();
            } else {
                (*it)->powerOn();
            }
        } else {
            Console::showError("Device not found");
        }
    }

    void connectDevices() {
        listDevices();
        if (devices.size() < 2) {
            Console::showError("Need at least 2 devices to connect");
            return;
        }
        
        string id1 = getValidInput("\nEnter first device ID", "", validateNotEmpty);
        string id2 = getValidInput("Enter second device ID", "", validateNotEmpty);
        
        auto device1 = find_if(devices.begin(), devices.end(), 
            [&id1](NetworkDevice* d) { return d->getID() == id1; });
        auto device2 = find_if(devices.begin(), devices.end(), 
            [&id2](NetworkDevice* d) { return d->getID() == id2; });
            
        if (device1 == devices.end() || device2 == devices.end()) {
            Console::showError("One or both devices not found");
            return;
        }
        
        // Check if devices are powered on
        if (!(*device1)->getPowerStatus() || !(*device2)->getPowerStatus()) {
            Console::showError("Both devices must be powered on to connect");
            return;
        }
        
        // Special handling for switch connections
        if ((*device1)->getType() == "Switch" && (*device2)->getType() != "Switch") {
            int portCount = dynamic_cast<Switch*>(*device1)->getPortCount();
            string port = getValidInput("Enter port number on switch (0-" + to_string(portCount-1) + ")", 
                                      "0", validatePositiveNumber);
            dynamic_cast<Switch*>(*device1)->connectDevice(stoi(port), (*device2)->getID());
            Console::showSuccess("Connected " + (*device2)->getName() + " to switch port " + port);
        } 
        else if ((*device2)->getType() == "Switch" && (*device1)->getType() != "Switch") {
            int portCount = dynamic_cast<Switch*>(*device2)->getPortCount();
            string port = getValidInput("Enter port number on switch (0-" + to_string(portCount-1) + ")", 
                                      "0", validatePositiveNumber);
            dynamic_cast<Switch*>(*device2)->connectDevice(stoi(port), (*device1)->getID());
            Console::showSuccess("Connected " + (*device1)->getName() + " to switch port " + port);
        }
        else {
            // Generic connection for other device types
            if ((*device1)->getType() == "Router") {
                dynamic_cast<Router*>(*device1)->connectDevice((*device2)->getID());
            }
            if ((*device2)->getType() == "Router") {
                dynamic_cast<Router*>(*device2)->connectDevice((*device1)->getID());
            }
            Console::showSuccess("Connected " + (*device1)->getName() + " and " + (*device2)->getName());
        }
    }

    void simulationMenu() {
        while (true) {
            Console::clear();
            Console::drawHeader("NETWORK SIMULATION");
            
            vector<string> menu = {
                "Send Network Traffic",
                "Simulate Router Operations",
                "Simulate Switch Operations",
                "Simulate Firewall Operations",
                "Run Complete Simulation",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-6)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: simulateTraffic(); break;
                case 2: simulateRouterOps(); break;
                case 3: simulateSwitchOps(); break;
                case 4: simulateFirewallOps(); break;
                case 5: runCompleteSimulation(); break;
                case 6: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }

    void simulateTraffic() {
        Console::clear();
        Console::drawHeader("SIMULATE NETWORK TRAFFIC");
        
        if (devices.empty()) {
            Console::showError("No devices to simulate traffic");
            return;
        }
        
        string countStr = getValidInput("Enter number of packets to simulate", "10", validatePositiveNumber);
        int count = stoi(countStr);
        
        Console::setColor(Console::LIGHT_BLUE);
        cout << "\nSimulating " << count << " packets across network...\n";
        
        for (int i = 0; i < count; i++) {
            Console::sleep(100);
            Console::drawProgressBar((i+1)*100/count);
            
            // Randomly select a device to process traffic
            if (!devices.empty()) {
                int idx = rand() % devices.size();
                string type = devices[idx]->getType();
                
                if (type == "Router") {
                    dynamic_cast<Router*>(devices[idx])->forwardPacket(1);
                } 
                else if (type == "Switch") {
                    dynamic_cast<Switch*>(devices[idx])->forwardFrame(1);
                }
                else if (type == "Firewall") {
                    if (rand() % 5 == 0) { // 20% chance of attack
                        dynamic_cast<Firewall*>(devices[idx])->blockAttack(1);
                    }
                }
            }
        }
        
        Console::showSuccess("\nTraffic simulation completed");
    }

    void simulateRouterOps() {
        Console::clear();
        Console::drawHeader("SIMULATE ROUTER OPERATIONS");
        
        vector<Router*> routers;
        for (auto device : devices) {
            if (device->getType() == "Router") {
                routers.push_back(dynamic_cast<Router*>(device));
            }
        }
        
        if (routers.empty()) {
            Console::showError("No routers in the network");
            return;
        }
        
        string countStr = getValidInput("Enter number of operations to simulate", "5", validatePositiveNumber);
        int count = stoi(countStr);
        
        for (int i = 0; i < count; i++) {
            Router* router = routers[rand() % routers.size()];
            
            // Random operation
            int op = rand() % 3;
            switch (op) {
                case 0: {
                    // Add route
                    string network = to_string(rand() % 256) + "." + to_string(rand() % 256) + ".0.0";
                    string gateway = "192.168." + to_string(rand() % 256) + ".1";
                    router->addRoute(network, gateway);
                    break;
                }
                case 1: {
                    // Forward packets
                    int packets = 1 + rand() % 10;
                    router->forwardPacket(packets);
                    break;
                }
                case 2: {
                    // Connect to random device
                    if (!devices.empty()) {
                        NetworkDevice* other = devices[rand() % devices.size()];
                        if (other != router) {
                            router->connectDevice(other->getID());
                            Console::showInfo("Connected " + router->getName() + " to " + other->getName());
                        }
                    }
                    break;
                }
            }
            Console::sleep(500);
        }
    }

    void simulateSwitchOps() {
        Console::clear();
        Console::drawHeader("SIMULATE SWITCH OPERATIONS");
        
        vector<Switch*> switches;
        for (auto device : devices) {
            if (device->getType() == "Switch") {
                switches.push_back(dynamic_cast<Switch*>(device));
            }
        }
        
        if (switches.empty()) {
            Console::showError("No switches in the network");
            return;
        }
        
        string countStr = getValidInput("Enter number of operations to simulate", "5", validatePositiveNumber);
        int count = stoi(countStr);
        
        for (int i = 0; i < count; i++) {
            Switch* sw = switches[rand() % switches.size()];
            
            // Random operation
            int op = rand() % 3;
            switch (op) {
                case 0: {
                    // Forward frames
                    int frames = 1 + rand() % 10;
                    sw->forwardFrame(frames);
                    break;
                }
                case 1: {
                    // Connect to random device
                    if (!devices.empty()) {
                        NetworkDevice* other = devices[rand() % devices.size()];
                        if (other != sw) {
                            int port = rand() % sw->getPortCount();
                            sw->connectDevice(port, other->getID());
                            Console::showInfo("Connected " + other->getName() + " to " + 
                                            sw->getName() + " port " + to_string(port));
                        }
                    }
                    break;
                }
                case 2: {
                    // Toggle spanning tree
                    bool newState = !sw->getSpanningTreeStatus();
                    Console::showInfo((newState ? "Enabled" : "Disabled") + 
                                     string(" spanning tree on ") + sw->getName());
                    break;
                }
            }
            Console::sleep(500);
        }
    }

    void simulateFirewallOps() {
        Console::clear();
        Console::drawHeader("SIMULATE FIREWALL OPERATIONS");
        
        vector<Firewall*> firewalls;
        for (auto device : devices) {
            if (device->getType() == "Firewall") {
                firewalls.push_back(dynamic_cast<Firewall*>(device));
            }
        }
        
        if (firewalls.empty()) {
            Console::showError("No firewalls in the network");
            return;
        }
        
        string countStr = getValidInput("Enter number of operations to simulate", "5", validatePositiveNumber);
        int count = stoi(countStr);
        
        for (int i = 0; i < count; i++) {
            Firewall* fw = firewalls[rand() % firewalls.size()];
            
            // Random operation
            int op = rand() % 3;
            switch (op) {
                case 0: {
                    // Block attack
                    int attacks = 1 + rand() % 5;
                    fw->blockAttack(attacks);
                    break;
                }
                case 1: {
                    // Add rule
                    vector<string> services = {"HTTP", "HTTPS", "SSH", "FTP", "DNS", "SMTP", "RDP"};
                    string service = services[rand() % services.size()];
                    bool allow = rand() % 2 == 0;
                    fw->addRule(service, allow);
                    Console::showInfo((allow ? "Allowed" : "Blocked") + 
                                     string(" service ") + service + " on " + fw->getName());
                    break;
                }
                case 2: {
                    // Toggle IDS
                    bool newState = !fw->getIdsStatus();
                    Console::showInfo((newState ? "Enabled" : "Disabled") + 
                                     string(" IDS on ") + fw->getName());
                    break;
                }
            }
            Console::sleep(500);
        }
    }

    void runCompleteSimulation() {
        Console::clear();
        Console::drawHeader("COMPLETE NETWORK SIMULATION");
        
        string durationStr = getValidInput("Enter simulation duration (seconds)", "10", validatePositiveNumber);
        int duration = stoi(durationStr);
        
        Console::setColor(Console::LIGHT_MAGENTA);
        cout << "\nRunning complete network simulation for " << duration << " seconds...\n";
        Console::reset();
        
        time_t startTime = time(nullptr);
        while (difftime(time(nullptr), startTime) < duration) {
            // Randomly select a simulation type
            int simType = rand() % 4;
            switch (simType) {
                case 0: simulateTraffic(); break;
                case 1: simulateRouterOps(); break;
                case 2: simulateSwitchOps(); break;
                case 3: simulateFirewallOps(); break;
            }
            Console::sleep(1000);
        }
        
        Console::showSuccess("\nSimulation completed successfully");
    }

    void diagnosticsMenu() {
        while (true) {
            Console::clear();
            Console::drawHeader("NETWORK DIAGNOSTICS");
            
            vector<string> menu = {
                "Run Device Diagnostics",
                "Run Detailed Diagnostics",
                "Check Network Health",
                "View System Logs",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-5)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: runDeviceDiagnostics(false); break;
                case 2: runDeviceDiagnostics(true); break;
                case 3: checkNetworkHealth(); break;
                case 4: Logger::viewLog(); break;
                case 5: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }

    void runDeviceDiagnostics(bool detailed) {
        listDevices();
        if (devices.empty()) return;
        
        string id = getValidInput("\nEnter device ID to diagnose", "", validateNotEmpty);
        
        auto it = find_if(devices.begin(), devices.end(), 
            [&id](NetworkDevice* d) { return d->getID() == id; });
            
        if (it != devices.end()) {
            (*it)->runDiagnostics(detailed);
        } else {
            Console::showError("Device not found");
        }
    }

    void checkNetworkHealth() {
        Console::clear();
        Console::drawHeader("NETWORK HEALTH CHECK");
        
        if (devices.empty()) {
            Console::showError("No devices in the network");
            return;
        }
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\nChecking network health...\n";
        
        int onlineDevices = 0;
        int issuesFound = 0;
        
        for (auto device : devices) {
            if (device->getPowerStatus()) {
                onlineDevices++;
                
                // Check for potential issues
                if (device->getType() == "Router") {
                    Router* r = dynamic_cast<Router*>(device);
                    if (r->getPacketsForwarded() == 0) {
                        Console::showWarning(device->getName() + " has not forwarded any packets");
                        issuesFound++;
                    }
                }
                else if (device->getType() == "Switch") {
                    Switch* s = dynamic_cast<Switch*>(device);
                    if (s->getFramesForwarded() == 0) {
                        Console::showWarning(device->getName() + " has not forwarded any frames");
                        issuesFound++;
                    }
                }
                else if (device->getType() == "Firewall") {
                    Firewall* f = dynamic_cast<Firewall*>(device);
                    if (!f->getIdsStatus()) {
                        Console::showWarning(device->getName() + " has IDS disabled");
                        issuesFound++;
                    }
                }
            }
        }
        
        double onlinePercentage = (devices.size() > 0) ? 
            (static_cast<double>(onlineDevices) / devices.size()) * 100 : 0;
        
        Console::setColor(Console::LIGHT_GREEN);
        cout << "\nNetwork Health Summary:\n";
        cout << "Online devices: " << onlineDevices << "/" << devices.size() 
             << " (" << fixed << setprecision(1) << onlinePercentage << "%)\n";
        cout << "Issues detected: " << issuesFound << endl;
        
        if (onlinePercentage < 50) {
            Console::showError("CRITICAL: More than half of devices are offline!");
        } else if (issuesFound > 3) {
            Console::showWarning("WARNING: Multiple issues detected in network");
        } else if (onlinePercentage == 100 && issuesFound == 0) {
            Console::showSuccess("Network is in optimal condition");
        } else {
            Console::showInfo("Network is operational with minor issues");
        }
        
        Console::reset();
    }

    void statisticsMenu() {
        while (true) {
            Console::clear();
            Console::drawHeader("NETWORK STATISTICS");
            
            vector<string> menu = {
                "View Device Statistics",
                "View Router Statistics",
                "View Switch Statistics",
                "View Firewall Statistics",
                "View Network Topology",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-6)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: viewDeviceStats(); break;
                case 2: viewRouterStats(); break;
                case 3: viewSwitchStats(); break;
                case 4: viewFirewallStats(); break;
                case 5: viewTopology(); break;
                case 6: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }

    void viewDeviceStats() {
        Console::clear();
        Console::drawHeader("DEVICE STATISTICS");
        
        if (devices.empty()) {
            Console::showInfo("No devices in the network");
            return;
        }
        
        int routers = Router::getTotalRouters();
        int switches = Switch::getTotalSwitches();
        int firewalls = Firewall::getTotalFirewalls();
        int active = NetworkDevice::getActiveDevices();
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\nNetwork Device Summary:\n";
        cout << string(40, '-') << endl;
        Console::setColor(Console::WHITE);
        cout << left << setw(20) << "Total Devices:" << devices.size() << endl;
        cout << left << setw(20) << "Active Devices:" << active << endl;
        cout << left << setw(20) << "Routers:" << routers << endl;
        cout << left << setw(20) << "Switches:" << switches << endl;
        cout << left << setw(20) << "Firewalls:" << firewalls << endl;
        
        // Device with longest uptime
        time_t longestUptime = 0;
        NetworkDevice* longestDevice = nullptr;
        
        for (auto device : devices) {
            if (device->getPowerStatus()) {
                time_t uptime = time(nullptr) - device->getPowerOnTime();
                if (uptime > longestUptime) {
                    longestUptime = uptime;
                    longestDevice = device;
                }
            }
        }
        
        if (longestDevice) {
            cout << left << setw(20) << "Longest Uptime:" 
                 << longestDevice->getName() << " (" << longestDevice->getUptime() << ")\n";
        }
        
        Console::reset();
    }

    void viewRouterStats() {
        Console::clear();
        Console::drawHeader("ROUTER STATISTICS");
        
        vector<Router*> routers;
        for (auto device : devices) {
            if (device->getType() == "Router") {
                routers.push_back(dynamic_cast<Router*>(device));
            }
        }
        
        if (routers.empty()) {
            Console::showInfo("No routers in the network");
            return;
        }
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\nRouter Statistics:\n";
        cout << string(60, '-') << endl;
        Console::setColor(Console::WHITE);
        cout << left << setw(20) << "Name" << setw(15) << "Status" 
             << setw(15) << "Packets" << setw(10) << "Routes" << endl;
        cout << string(60, '-') << endl;
        
        int totalPackets = 0;
        int totalRoutes = 0;
        
        for (auto router : routers) {
            cout << left << setw(20) << router->getName();
            
            if (router->getPowerStatus()) {
                Console::setColor(Console::LIGHT_GREEN);
                cout << setw(15) << "ONLINE";
            } else {
                Console::setColor(Console::LIGHT_RED);
                cout << setw(15) << "OFFLINE";
            }
            
            Console::setColor(Console::WHITE);
            cout << setw(15) << router->getPacketsForwarded() 
                 << setw(10) << router->getRoutingTableSize() << endl;
            
            if (router->getPowerStatus()) {
                totalPackets += router->getPacketsForwarded();
                totalRoutes += router->getRoutingTableSize();
            }
        }
        
        cout << string(60, '-') << endl;
        cout << left << setw(35) << "Totals (online devices only):" 
             << totalPackets << " packets, " << totalRoutes << " routes\n";
        
        Console::reset();
    }

    void viewSwitchStats() {
        Console::clear();
        Console::drawHeader("SWITCH STATISTICS");
        
        vector<Switch*> switches;
        for (auto device : devices) {
            if (device->getType() == "Switch") {
                switches.push_back(dynamic_cast<Switch*>(device));
            }
        }
        
        if (switches.empty()) {
            Console::showInfo("No switches in the network");
            return;
        }
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\nSwitch Statistics:\n";
        cout << string(60, '-') << endl;
        Console::setColor(Console::WHITE);
        cout << left << setw(20) << "Name" << setw(15) << "Status" 
             << setw(15) << "Frames" << setw(10) << "Ports" << endl;
        cout << string(60, '-') << endl;
        
        int totalFrames = 0;
        int totalPorts = 0;
        int activePorts = 0;
        
        for (auto sw : switches) {
            cout << left << setw(20) << sw->getName();
            
            if (sw->getPowerStatus()) {
                Console::setColor(Console::LIGHT_GREEN);
                cout << setw(15) << "ONLINE";
            } else {
                Console::setColor(Console::LIGHT_RED);
                cout << setw(15) << "OFFLINE";
            }
            
            Console::setColor(Console::WHITE);
            cout << setw(15) << sw->getFramesForwarded() 
                 << setw(10) << sw->getPortCount() << endl;
            
            if (sw->getPowerStatus()) {
                totalFrames += sw->getFramesForwarded();
                totalPorts += sw->getPortCount();
                activePorts += sw->countActivePorts();
            }
        }
        
        cout << string(60, '-') << endl;
        double portUtilization = (totalPorts > 0) ? 
            (static_cast<double>(activePorts) / totalPorts * 100) : 0;
            
        cout << left << setw(35) << "Totals (online devices only):" 
             << totalFrames << " frames, " << activePorts << "/" << totalPorts 
             << " ports active (" << fixed << setprecision(1) << portUtilization << "%)\n";
        
        Console::reset();
    }

    void viewFirewallStats() {
        Console::clear();
        Console::drawHeader("FIREWALL STATISTICS");
        
        vector<Firewall*> firewalls;
        for (auto device : devices) {
            if (device->getType() == "Firewall") {
                firewalls.push_back(dynamic_cast<Firewall*>(device));
            }
        }
        
        if (firewalls.empty()) {
            Console::showInfo("No firewalls in the network");
            return;
        }
        
        Console::setColor(Console::LIGHT_CYAN);
        cout << "\nFirewall Statistics:\n";
        cout << string(60, '-') << endl;
        Console::setColor(Console::WHITE);
        cout << left << setw(20) << "Name" << setw(15) << "Status" 
             << setw(15) << "Blocks" << setw(10) << "Rules" << endl;
        cout << string(60, '-') << endl;
        
        int totalBlocks = 0;
        int totalRules = 0;
        
        for (auto fw : firewalls) {
            cout << left << setw(20) << fw->getName();
            
            if (fw->getPowerStatus()) {
                Console::setColor(Console::LIGHT_GREEN);
                cout << setw(15) << "ONLINE";
            } else {
                Console::setColor(Console::LIGHT_RED);
                cout << setw(15) << "OFFLINE";
            }
            
            Console::setColor(Console::WHITE);
            cout << setw(15) << fw->getBlockedAttempts() 
                 << setw(10) << fw->getSecurityRulesCount() << endl;
            
            if (fw->getPowerStatus()) {
                totalBlocks += fw->getBlockedAttempts();
                totalRules += fw->getSecurityRulesCount();
            }
        }
        
        cout << string(60, '-') << endl;
        cout << left << setw(35) << "Totals (online devices only):" 
             << totalBlocks << " blocks, " << totalRules << " rules\n";
        
        Console::reset();
    }

    void viewTopology() {
        Console::clear();
        Console::drawHeader("NETWORK TOPOLOGY");
        
        if (devices.empty()) {
            Console::showInfo("No devices in the network");
            return;
        }
        
        vector<string> deviceNames;
        for (auto device : devices) {
            deviceNames.push_back(device->getName() + " (" + device->getType() + ")");
        }
        
        Console::drawNetworkTopology(deviceNames);
    }

    void adminMenu() {
        if (!authenticateAdmin()) {
            pressEnterToContinue();
            return;
        }
        
        while (true) {
            Console::clear();
            Console::drawHeader("ADMIN FUNCTIONS");
            
            vector<string> menu = {
                "View System Logs",
                "Clear Logs",
                "Backup Configuration",
                "Restore Configuration",
                "Change Admin Password",
                "Reset Simulator",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-7)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: Logger::viewLog(); break;
                case 2: Logger::clearLog(); break;
                case 3: backupConfig(); break;
                case 4: restoreConfig(); break;
                case 5: changeAdminPassword(); break;
                case 6: resetSimulator(); break;
                case 7: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }

    void backupConfig() {
        ofstream outFile(CONFIG_FILE);
        if (outFile.is_open()) {
            outFile << "NETWORK_CONFIG_VERSION=1.0\n";
            outFile << "TOTAL_DEVICES=" << devices.size() << "\n";
            
            for (auto device : devices) {
                outFile << device->toFileString() << "\n";
            }
            
            outFile.close();
            Console::showSuccess("Configuration saved to " + CONFIG_FILE);
            Logger::log("INFO: Configuration backed up to " + CONFIG_FILE);
        } else {
            Console::showError("Failed to save configuration");
            Logger::log("ERROR: Failed to backup configuration");
        }
    }

    void restoreConfig() {
        ifstream inFile(CONFIG_FILE);
        if (!inFile.is_open()) {
            Console::showError("No configuration file found");
            return;
        }
        
        // Clear existing devices
        for (auto device : devices) {
            delete device;
        }
        devices.clear();
        
        string line;
        bool versionChecked = false;
        
        while (getline(inFile, line)) {
            if (!versionChecked) {
                if (line.find("NETWORK_CONFIG_VERSION") == string::npos) {
                    Console::showError("Invalid configuration file format");
                    Logger::log("ERROR: Invalid config file format during restore");
                    return;
                }
                versionChecked = true;
                continue;
            }
            
            if (line.find("TOTAL_DEVICES") != string::npos) {
                continue; // We'll process devices regardless of this count
            }
            
            // Parse device data
            size_t typeEnd = line.find(',');
            if (typeEnd == string::npos) continue;
            
            string type = line.substr(0, typeEnd);
            NetworkDevice* newDevice = nullptr;
            
            if (type == "Router") {
                newDevice = new Router("temp", "temp", "temp");
            } 
            else if (type == "Switch") {
                newDevice = new Switch("temp", 1, "temp", "temp");
            }
            else if (type == "Firewall") {
                newDevice = new Firewall("temp", "temp", "temp");
            }
            
            if (newDevice) {
                newDevice->fromFileString(line);
                devices.push_back(newDevice);
            }
        }
        
        inFile.close();
        Console::showSuccess("Configuration restored from " + CONFIG_FILE);
        Logger::log("INFO: Configuration restored from " + CONFIG_FILE);
    }

    void changeAdminPassword() {
        string newPass = getValidInput("Enter new admin password", "", validateNotEmpty);
        string confirmPass = getValidInput("Confirm new password", "", validateNotEmpty);
        
        if (newPass == confirmPass) {
            adminPassword = newPass;
            Console::showSuccess("Admin password changed successfully");
                       Logger::log("INFO: Admin password changed");
        } else {
            Console::showError("Passwords do not match");
        }
    }

    void resetSimulator() {
        string confirm = getValidInput("Are you sure you want to reset? (y/n)", "n");
        if (tolower(confirm[0]) == 'y') {
            for (auto device : devices) {
                delete device;
            }
            devices.clear();
            Logger::clearLog();
            Console::showSuccess("Simulator has been reset");
            Logger::log("INFO: Simulator was reset by admin");
        }
    }

    void helpMenu() {
        while (true) {
            Console::clear();
            Console::drawHeader("HELP SYSTEM");
            
            vector<string> menu = {
                "General Help",
                "Router Help",
                "Switch Help",
                "Firewall Help",
                "Admin Help",
                "Back to Main Menu"
            };
            
            Console::drawMenuBox(menu);
            
            string choice = getValidInput("Select option (1-6)", "1", validatePositiveNumber);
            int option = stoi(choice);
            
            switch (option) {
                case 1: HelpSystem::showGeneralHelp(); break;
                case 2: HelpSystem::showDeviceHelp("Router"); break;
                case 3: HelpSystem::showDeviceHelp("Switch"); break;
                case 4: HelpSystem::showDeviceHelp("Firewall"); break;
                case 5: HelpSystem::showAdminHelp(); break;
                case 6: return;
                default: Console::showError("Invalid option"); break;
            }
            
            pressEnterToContinue();
        }
    }
};

// ========== Main Function ==========
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    
    Console::clear();
    Console::setColor(Console::LIGHT_CYAN);
    cout << "=== ENTERPRISE NETWORK SIMULATOR ===" << endl;
    Console::setColor(Console::LIGHT_BLUE);
    cout << "Initializing system...";
    
    for (int i = 0; i <= 100; i += 10) {
        Console::drawProgressBar(i);
        Console::sleep(100);
    }
    
    NetworkSimulator simulator;
    simulator.run();
    
    return 0;
}           