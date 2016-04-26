//
//  StartupViewController.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 16.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


enum StartupStatus {
    
    case Processing(message: String, color: UIColor)
    
    case Success(message: String)
    
    case Error(error: String)
}



let PROCESSING_COLOR = UIColor.whiteColor()

let SUCCESS_COLOR = UIColor.greenColor()

let ERROR_COLOR = UIColor.redColor()


private let PFD_CONTROLLER_STORYBOARD_ID = "pfdController"



class StartupViewController: UIViewController {

    
    // MARK: | IB Outlets
    
    @IBOutlet private weak var statusLabel: UILabel?
    
    @IBOutlet private weak var activityIndicator: UIActivityIndicatorView?

    @IBOutlet private weak var retryButton: UIButton?
    
    
    // MARK: | IB Actions
    
    @IBAction private func retryButtonPressed(sender: UIButton) {
    }
    
    
    // MARK: | Status
    
    var status = StartupStatus.Processing(message: "", color: PROCESSING_COLOR) {
        didSet {
            self.updateUI()
        }
        
    }
    
    
    private func updateUI() {
        switch status {
            
        case .Processing(let message, let color):
            self.statusLabel?.textColor = color
            self.activityIndicator?.color = color
            self.activityIndicator?.hidden = false
            self.activityIndicator?.startAnimating()
            self.retryButton?.hidden = true
            self.statusLabel?.text = message
            break
            
        case .Success(let message):
            self.statusLabel?.textColor = SUCCESS_COLOR
            self.activityIndicator?.hidden = true
            self.retryButton?.hidden = true
            self.statusLabel?.text = message
            break
            
        case .Error(let error):
            self.statusLabel?.textColor = ERROR_COLOR
            self.activityIndicator?.hidden = true
            self.retryButton?.hidden = false
            self.statusLabel?.text = error
            break
            
        }
    }


    override func viewDidLoad() {
        super.viewDidLoad()
        self.updateUI()
    }
    
    
    lazy var pfdController: PFDViewController = self.storyboard?.instantiateViewControllerWithIdentifier(PFD_CONTROLLER_STORYBOARD_ID) as! PFDViewController
}


extension StartupViewController: BLEConnectorDelegate {
    
    func connectorDiscoveredPFD(connector: BLEConnector) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            self.status = StartupStatus.Processing(message: "Establishing connection", color: PROCESSING_COLOR)
        }
    }
    
    
    func connectorEstablishedConnection(connector: BLEConnector) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            self.status = StartupStatus.Success(message: "Connection established")
            self.presentViewController(self.pfdController, animated: true, completion: nil)
        }
    }
    
    
    func connectorFailedConnecting(connector: BLEConnector) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
           self.status =  StartupStatus.Error(error: "Establishing connection failed")
        }
    }
    
    
    func connectorLostConnection(connector: BLEConnector) {
        dispatch_async(dispatch_get_main_queue()) { () -> Void in
            self.pfdController.dismissViewControllerAnimated(true, completion: nil)
            self.status = StartupStatus.Processing(message: "Lost connection,\nreconnecting", color: ERROR_COLOR)
        }
    }
    
}
