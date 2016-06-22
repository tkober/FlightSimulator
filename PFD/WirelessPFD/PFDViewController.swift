//
//  PFDViewController.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 10.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


let PURPLE_COLOR = UIColor(red: 255.0/255.0, green: 102.0/255.0, blue: 255.0/255.0, alpha: 1)
let GREEN_COLOR = UIColor(red: 0/255.0, green: 255.0/255.0, blue: 0/255.0, alpha: 1)
let BLUE_COLOR = UIColor(red: 102/255.0, green: 204.0/255.0, blue: 255.0/255.0, alpha: 1)
let GREY_COLOR = UIColor(red: 179.0/255.0, green: 179.0/255.0, blue: 179.0/255.0, alpha: 1)


// For Testing Only
let GROUND_CLEARANCE: Int = 2500


class PFDViewController: UIViewController {

    
    // MARK: - ADI
    
    @IBOutlet weak var attitudeIndicatorView: AttitudeIndicatorView!
    
    @IBOutlet weak var adiContainer: ADIContainerView!
    
    @IBOutlet weak var adiContainerCenterConstraint: NSLayoutConstraint!
    
    
    private let ilsSize: CGFloat = 30
    
    var ilsVisible = false
    
    
    func toggleILSVisibility() {
        if self.ilsVisible {
            self.hideILS(true)
        } else {
            self.showILS(true)
        }
    }
    
    
    func showILS(animated: Bool) {
        UIView.animateWithDuration(animated ? 0.5 : 0, animations: { () -> Void in
            
            self.adiContainerCenterConstraint.constant = -self.ilsSize / 2.0
            self.localizerHeightConstraint.constant = self.ilsSize
            self.glideslopeWidthConstraint.constant = self.ilsSize
            self.view.layoutIfNeeded()
            self.localizerScaleView.setNeedsDisplay()
            self.localizerNeedleView.setNeedsDisplay()
            self.glideslopeScaleView.setNeedsDisplay()
            self.glideslopeNeedleView.setNeedsDisplay()
            
        }) { (finished: Bool) -> Void in
            
            self.ilsVisible = true
            self.attitudeIndicatorView?.rerender()
            
        }
    }
    
    
    func hideILS(animated: Bool) {
        UIView.animateWithDuration(animated ? 0.3 : 0, animations: { () -> Void in
            
            self.adiContainerCenterConstraint.constant = 0
            self.localizerHeightConstraint.constant = 0
            self.glideslopeWidthConstraint.constant = 0
            self.view.layoutIfNeeded()
            
            }) { (finished: Bool) -> Void in
                
                self.ilsVisible = false
                self.attitudeIndicatorView?.rerender()
                
        }
    }
    
    
    // MARK: - ILS
    
    @IBOutlet weak var localizerScaleView: ILSLocalizerScaleView!
    
    @IBOutlet weak var localizerNeedleView: ILSLocalizerNeedleView!
    
    @IBOutlet weak var localizerHeightConstraint: NSLayoutConstraint!
    
    @IBOutlet weak var glideslopeScaleView: ILSGlideslopeScaleView!
    
    @IBOutlet weak var glideslopeNeedleView: ILSGlideslopeNeedleView!
    
    @IBOutlet weak var glideslopeWidthConstraint: NSLayoutConstraint!
    
    
    // MARK: | Ground Clearance
    
    
    @IBOutlet weak var groundClearanceLabel: UILabel!
    
    @IBOutlet weak var groundClearanceBackgroundTopConstraint: NSLayoutConstraint!
    
    @IBOutlet weak var groundClearanceBackgroundBottomConstraint: NSLayoutConstraint!
    
    var groundClearanceVisible = false
    
    
    func showGroundClearance(animated: Bool) {
        UIView.animateWithDuration(animated ? 0.5 : 0, animations: { () -> Void in
            
            self.groundClearanceLabel.text = "\(GROUND_CLEARANCE)"
            self.groundClearanceBackgroundTopConstraint.constant = self.adiOverlayPadding
            self.groundClearanceBackgroundBottomConstraint.constant = -self.adiOverlayPadding
            self.headingLabel.font = self.adiOverlayMinimizedFont
            self.view.layoutIfNeeded()
            self.headingBackgroundView.borderWidth = 0
            self.headingBackgroundView.setNeedsDisplay()
            self.groundClearanceVisible = true
            self.groundClearanceVisible = true
            
        }) { (finished: Bool) -> Void in
            
        }
    }
    
    
    func hideGroundClearance(animated: Bool) {
        UIView.animateWithDuration(animated ? 0.5 : 0, animations: { () -> Void in
            
            self.groundClearanceLabel.text = nil
            self.groundClearanceBackgroundTopConstraint.constant = 0
            self.groundClearanceBackgroundBottomConstraint.constant = 0
            self.headingLabel.font = self.adiOverlayFont
            self.view.layoutIfNeeded()
            self.headingBackgroundView.borderWidth = self.adiOverlayPadding
            self.headingBackgroundView.setNeedsDisplay()
            self.groundClearanceVisible = false
            
        }) { (finished: Bool) -> Void in
        
        }
    }
    
    
    // MARK: | Heading
    
    @IBOutlet weak var headingLabel: UILabel!
    
    @IBOutlet weak var headingBackgroundView: ADIOverlayBackgroundView!
    
    
    private let adiOverlayBackgroundBorderSize: CGFloat = 2
    
    private let adiOverlayPadding: CGFloat = 2
    
    private let adiOverlayFont = UIFont.boldSystemFontOfSize(30)
    
    private let adiOverlayMinimizedFont = UIFont.boldSystemFontOfSize(15)
    
    
    // MARK: | Altitude
    
    @IBOutlet weak var flightLevelLabel: UILabel?
    
    @IBOutlet weak var altitudeTrailingDigitsLabel: UILabel?
    
    
    // MARK: | Airspeed
    
    @IBOutlet weak var airspeedLabel: UILabel?
    
    @IBOutlet weak var airspeedScale: AirspeedScaleView?
    
    private var _airspeed: UInt = 0
    
    private func updateAirspeed() {
        self.airspeedLabel?.text = "\(self._airspeed)"
        airspeedScale?.setAirspeed(self._airspeed)
    }
    
    
    // MARK: | Views Lifecyle
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.adiContainer?.backgroundColor = UIColor.clearColor()
        self.hideILS(false)
        self.hideGroundClearance(false)
        self.view.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(toggleILSVisibility)))
        
        
        self.handlePFDValue(PFDValue.Airspeed(value: 150))
        self.handlePFDValue(PFDValue.GroundClearance(value: 2000))
        self.handlePFDValue(PFDValue.Altitude(value: 3000))
        self.handlePFDValue(PFDValue.Pitch(value: 3))
        self.handlePFDValue(PFDValue.Roll(value: 2))
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    
    // MARK: | Reference Speed
    
    @IBOutlet weak var referenceSpeedLabel: UILabel?
    
    private var _referenceSpeed: UInt = 0
    
    private var _speedHold = false
    
    private var _togaActive = false
    
    private func updateReferenceSpeedUI() {
        if _togaActive {
            self.referenceSpeedLabel?.text = "TO/GA"
            self.referenceSpeedLabel?.textColor = PURPLE_COLOR
        } else {
            self.referenceSpeedLabel?.text = "\(_referenceSpeed)"
            self.referenceSpeedLabel?.textColor = _speedHold ? PURPLE_COLOR : GREY_COLOR
        }
    }
    
    
    // MARK: | Reference Altitude
    
    @IBOutlet weak var referenceAltitudeLabel: UILabel?
    
    private var _referenceAltitude: Int = 0
    
    private var _altitudeHold = false
    
    private func updateReferenceAltitudeUI() {
        self.referenceAltitudeLabel?.text = "\(_referenceAltitude)"
        self.referenceAltitudeLabel?.textColor = _altitudeHold ? PURPLE_COLOR : GREY_COLOR
    }
    
    
    // MARK: | Altimeter Setting
    
    @IBOutlet weak var altimeterSettingLabel: UILabel?
    
    private var _altimeterSetting = "29.92"
    
    private func updateAltimeterSettingUI() {
        self.altimeterSettingLabel?.text = self._altimeterSetting == "29.92" ? "STD" : self._altimeterSetting
        self.altimeterSettingLabel?.textColor = self._altimeterSetting == "29.92" ? GREEN_COLOR : BLUE_COLOR
    }
}


/*

<Q+005.0<R-015.0<D04000<P280<J170

*/

extension PFDViewController: PFDValueHandler {
    
    func handlePFDValue(value: PFDValue) {
        switch value {
            
        case .Airspeed(let airspeed):
            self._airspeed = airspeed
            self.updateAirspeed()
            break
            
        case .Altitude(let altitude):
            let flightLevel = altitude / 100
            let flightLevelString: String
            if flightLevel < 100 {
                flightLevelString = "\(flightLevel < 10 ? "  " : " ")\(flightLevel)"
            } else {
                flightLevelString = "\(flightLevel)"
            }
            self.flightLevelLabel?.text = flightLevelString
            let trailingDigits = altitude % 100
            self.altitudeTrailingDigitsLabel?.text = "\(trailingDigits < 10 ? "0" : "")\(trailingDigits)"
            break
            
        case .Heading(let heading):
            self.headingLabel.text = "\(heading)°"
            break
            
        case .GroundClearance(let groundClearance):
            
            if groundClearance <= 2500 {
                if !self.groundClearanceVisible {
                    self.showGroundClearance(true)
                }
                self.groundClearanceLabel.text = "\(groundClearance)"
            } else {
                if self.groundClearanceVisible {
                    self.hideGroundClearance(true)
                }
            }
            break
            
        case .ILSLocOffset(let locOffset):
            self.localizerNeedleView.setPosition(locOffset)
            break
            
        case .ILSGSOffset(let gsOffset):
            self.glideslopeNeedleView.setPosition(gsOffset)
            break;
            
        case .ReferenceSpeed(let referenceSpeed):
            self._referenceSpeed = referenceSpeed
            self.updateReferenceSpeedUI()
            break
            
        case .SpeedHold(let speedHold):
            self._speedHold = speedHold
            self.updateReferenceSpeedUI()
            break
            
        case .ReferenceAltitude(let referenceAltitude):
            self._referenceAltitude = referenceAltitude
            self.updateReferenceAltitudeUI()
            break
            
        case .AltitudeHold(let altitudeHold):
            self._altitudeHold = altitudeHold
            self.updateReferenceAltitudeUI()
            break
            
        case .Pitch(let pitch):
            self.attitudeIndicatorView.setPitch(pitch)
            break
            
        case .Roll(let roll):
            self.attitudeIndicatorView.setBankAngle(roll)
            break
            
        case .AltimeterSetting(let altimeterSetting):
            self._altimeterSetting = altimeterSetting
            self.updateAltimeterSettingUI()
            break
            
        case .TOGAActive(let togaActive):
            _togaActive = togaActive
            self.updateReferenceSpeedUI()
            break
            
            
            
        case .Unknown:
            break
        }
    }
}




