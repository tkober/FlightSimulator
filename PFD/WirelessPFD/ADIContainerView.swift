//
//  ADIContainerView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 09.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


@IBDesignable class ADIContainerView: UIView {
    
    
    @IBInspectable var cornerRadius: CGFloat = 0.05
    
    
    override func drawRect(rect: CGRect) {
        super.drawRect(rect)
        
        self.layer.cornerRadius = min(rect.width, rect.height) * self.cornerRadius
        self.clipsToBounds = true
    }
    
}