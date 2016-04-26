//
//  BLEConnector.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 15.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import Foundation
import CoreBluetooth


protocol BLEConnectorDelegate {
    
    func connectorDiscoveredPFD(connector: BLEConnector)
    
    func connectorEstablishedConnection(connector: BLEConnector)
    
    func connectorFailedConnecting(connector: BLEConnector)
    
    func connectorLostConnection(connector: BLEConnector)
}


class BLEConnector: NSObject {
    
    
    var delegate: BLEConnectorDelegate?
    
    
    // MARK: | UUIDs
    
    private let PFD_SERVICE_UUID = CBUUID(string: "713D0000-503E-4C75-BA94-3148F18D941E")
    
    private let PFD_UART_READ_CHARACTERISTIC_UUID = CBUUID(string: "713D0002-503E-4C75-BA94-3148F18D941E")
    
    
    // MARK: | Shared Instance
    
    class var sharedInstance : BLEConnector {
        struct Static {
            static var onceToken: dispatch_once_t = 0
            static var instance: BLEConnector? = nil
        }
        
        dispatch_once(&Static.onceToken) {
            Static.instance = BLEConnector()
        }
        return Static.instance!
    }
    
    
    // MARK: | Central Manager
    
    
    private lazy var centralManager: CBCentralManager = CBCentralManager(delegate: self, queue: dispatch_queue_create("ble_queue", DISPATCH_QUEUE_SERIAL))
    
    
    // MARK: | Starting
    
    
    var startBLE = false
    
    
    func start() {
        if self.centralManager.state == CBCentralManagerState.PoweredOn {
            self.centralManager.scanForPeripheralsWithServices([PFD_SERVICE_UUID], options: nil)
            self.startBLE = false
        } else {
            self.startBLE = true
        }
    }
    
    
    var peripheral: CBPeripheral!
    
    var uartReadCharacteristic: CBCharacteristic!
    
    var pfdValueHandler: PFDValueHandler?
}


extension BLEConnector: CBCentralManagerDelegate {
    
    
    func centralManagerDidUpdateState(central: CBCentralManager) {
        switch central.state {
            
        case CBCentralManagerState.Unknown:
            break
            
        case CBCentralManagerState.Resetting:
            break
            
        case CBCentralManagerState.Unsupported:
            break
            
        case CBCentralManagerState.Unauthorized:
            break
            
        case CBCentralManagerState.PoweredOff:
            break
            
        case CBCentralManagerState.PoweredOn:
            if (self.startBLE) {
                self.start()
            }
            break
            
        }
    }
    
    
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        central.stopScan()
        self.delegate?.connectorDiscoveredPFD(self)
        self.peripheral = peripheral
        centralManager.connectPeripheral(peripheral, options: nil)
    }
    
    
    func centralManager(central: CBCentralManager, didConnectPeripheral peripheral: CBPeripheral) {
        peripheral.delegate = self
        peripheral.discoverServices([PFD_SERVICE_UUID])
    }
    
    
    func centralManager(central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: NSError?) {
        central.connectPeripheral(peripheral, options: nil)
        self.delegate?.connectorLostConnection(self)
    }
}



extension BLEConnector: CBPeripheralDelegate {
    
    func peripheral(peripheral: CBPeripheral, didDiscoverServices error: NSError?) {
        if error != nil {
            self.delegate?.connectorFailedConnecting(self)
            return
        }
        for service in peripheral.services! {
            if service.UUID == PFD_SERVICE_UUID {
                peripheral.discoverCharacteristics([], forService: service)
                return
            }
        }
    }
    
    
    func peripheral(peripheral: CBPeripheral, didDiscoverCharacteristicsForService service: CBService, error: NSError?) {
        if error != nil {
            self.delegate?.connectorFailedConnecting(self)
            return
        }
        for characteristic in service.characteristics! {
            if characteristic.UUID == PFD_UART_READ_CHARACTERISTIC_UUID {
                self.uartReadCharacteristic = characteristic
                peripheral.setNotifyValue(true, forCharacteristic: characteristic)
                return
            }
        }
    }
    
    
    func peripheral(peripheral: CBPeripheral, didUpdateNotificationStateForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        self.delegate?.connectorEstablishedConnection(self)
    }
    
    
    func peripheral(peripheral: CBPeripheral, didUpdateValueForCharacteristic characteristic: CBCharacteristic, error: NSError?) {
        if let characteristicValue = characteristic.value {
            let parser = DataParser(data: characteristicValue)
            let pfdValue = parser.parse()
            
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.pfdValueHandler?.handlePFDValue(pfdValue)
            }
        }
    }
}


