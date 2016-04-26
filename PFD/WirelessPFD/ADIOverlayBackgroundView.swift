//
//  ADIOverlayBackgroundView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 10.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


@IBDesignable class ADIOverlayBackgroundView : UIView {
    
    @IBInspectable var borderColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var borderWidth: CGFloat = 1
    
    
    override func drawRect(rect: CGRect) {
        self.layer.borderColor = self.borderColor.CGColor
        self.layer.borderWidth = self.borderWidth
    }
    
}
