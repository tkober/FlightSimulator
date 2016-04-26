//
//  DataParser.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 16.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import Foundation


enum PFDValue {
    
    case Airspeed(value: UInt)
    
    case Altitude(value: Int)
    
    case Heading(value: UInt)
    
    case GroundClearance(value: UInt)
    
    case ILSLocOffset(value: Int)
    
    case ILSGSOffset(value: Int)
    
    case ReferenceSpeed(value: UInt)
    
    case SpeedHold(value: Bool)
    
    case ReferenceAltitude(value: Int)
    
    case AltitudeHold(value: Bool)
    
    case Pitch(value: Double)
    
    case Roll(value: Double)
    
    case AltimeterSetting(value: String)
    
    case TOGAActive(value: Bool)
    
    
    
    case Unknown
}



typealias RuleProcessing = (string: NSString) -> PFDValue
typealias ParsingRules = [UInt8: RuleProcessing]



protocol PFDValueHandler {
    
    func handlePFDValue(value: PFDValue)
    
}



class DataParser: NSObject {

    // MARK: | Data
    
    var data: NSData
    
    
    // MARK: | Initializer
    
    init(data: NSData) {
        self.data = data
    }
    
    
    // MARK: | Parsing
    
    func parse() -> PFDValue {
        var key: UInt8 = 0x00
        self.data.getBytes(&key, length: 1)
        
        if let rule = self.parsingRules[key] {
            let payload = self.data.subdataWithRange(NSMakeRange(1, self.data.length - 1))
            NSCharacterSet.decimalDigitCharacterSet().invertedSet
            if let string = String(data: payload, encoding: NSUTF8StringEncoding) {
                if string.rangeOfCharacterFromSet(NSCharacterSet(charactersInString: "=<?")) == nil {
                    return rule(string: string)
                }
            }
            return PFDValue.Unknown
        }
        
        return PFDValue.Unknown
    }
    
    
    private lazy var parsingRules: ParsingRules = [
        
        // Airspeed
        0x01: {(string: NSString) -> (PFDValue) in
            return PFDValue.Airspeed(value: UInt((string as NSString).integerValue))
        },
        
        // Altitude 
        0x03: {(string: NSString) -> (PFDValue) in
            return PFDValue.Altitude(value: (string as NSString).integerValue)
        },
        
        // Heading
        0x04: {(string: NSString) -> (PFDValue) in
            return PFDValue.Heading(value: UInt((string as NSString).integerValue))
        },
        
        // Ground Clearance
        0x05: {(string: NSString) -> (PFDValue) in
            return PFDValue.GroundClearance(value: UInt((string as NSString).integerValue))
        },
        
        // ILS Loc Offset
        0x07: {(string: NSString) -> (PFDValue) in
            return PFDValue.ILSLocOffset(value: (string as NSString).integerValue)
        },
        
        // ILS GS Offset
        0x08: {(string: NSString) -> (PFDValue) in
            return PFDValue.ILSGSOffset(value: (string as NSString).integerValue)
        },
        
        // Reference Speed
        0x09: {(string: NSString) -> (PFDValue) in
            return PFDValue.ReferenceSpeed(value: UInt((string as NSString).integerValue))
        },
        
        // Speed Hold
        0x0A: {(string: NSString) -> (PFDValue) in
            return PFDValue.SpeedHold(value: Bool((string as NSString).integerValue))
        },
        
        // Reference Altitude
        0x0B: {(string: NSString) -> (PFDValue) in
            return PFDValue.ReferenceAltitude(value: (string as NSString).integerValue)
        },
        
        // Altitude Hold
        0x0C: {(string: NSString) -> (PFDValue) in
            return PFDValue.AltitudeHold(value: Bool((string as NSString).integerValue))
        },
       
        // Pitch
        0x0D: {(string: NSString) -> (PFDValue) in
            return PFDValue.Pitch(value: (string.stringByReplacingOccurrencesOfString(",", withString: ".") as NSString).doubleValue)
        },
        
        // Roll
        0x0E: {(string: NSString) -> (PFDValue) in
            return PFDValue.Roll(value: (string.stringByReplacingOccurrencesOfString(",", withString: ".") as NSString).doubleValue)
        },
        
        // Altimeter Setting
        0x0F: {(string: NSString) -> (PFDValue) in
            return PFDValue.AltimeterSetting(value: string.stringByReplacingOccurrencesOfString(",", withString: "."))
        },
        
        0x14: {(string: NSString) -> (PFDValue) in
            return PFDValue.TOGAActive(value: Bool((string as NSString).integerValue))
        }
    ]
}
