//
//  ILSView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 09.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


class ILSScaleView : UIView {
    
    @IBInspectable var scaleColor: UIColor = UIColor.whiteColor()
    
    
    @IBInspectable var scaleWidth: CGFloat = 2
    
    
    @IBInspectable var markerCount: UInt = 2
    
    
    @IBInspectable var centerMargin: CGFloat = 0.1
    
    
    @IBInspectable var markerMargin: CGFloat = 0.3
}



@IBDesignable class ILSLocalizerScaleView : ILSScaleView {
    
    override func drawRect(rect: CGRect) {
        
        let intervalSize = rect.width / CGFloat((self.markerCount * 2) + 1)
        let middle = ceil(rect.width / 2.0)
        let margin = floor(rect.height * self.centerMargin)
        
        let context = UIGraphicsGetCurrentContext()
        
        CGContextSetStrokeColorWithColor(context, self.scaleColor.CGColor)
        CGContextSetLineWidth(context, self.scaleWidth)
        
        // Middle Marker
        CGContextMoveToPoint(context, middle, margin)
        CGContextAddLineToPoint(context, middle, rect.height - margin)
        
        // Left and Right Markers
        let y = rect.height / 2.0
        let diameter = rect.height - (2 * (self.markerMargin * rect.height))
        let radius = diameter / 2.0
        var leftX = middle
        var rightX = middle
        for var i: UInt = 1; i <= self.markerCount; i++ {
            leftX -= intervalSize
            rightX += intervalSize
            
            CGContextAddEllipseInRect(context, CGRect(x: leftX - radius, y: y - radius, width: diameter, height: diameter))
            CGContextAddEllipseInRect(context, CGRect(x: rightX - radius, y: y - radius, width: diameter, height: diameter))
        }
        
        CGContextStrokePath(context)

    }
}



@IBDesignable class ILSGlideslopeScaleView : ILSScaleView {
    
    override func drawRect(rect: CGRect) {
        
        let intervalSize = rect.height / CGFloat((self.markerCount * 2) + 1)
        let middle = ceil(rect.height / 2.0)
        let margin = floor(rect.width * self.centerMargin)
        
        let context = UIGraphicsGetCurrentContext()
        
        CGContextSetStrokeColorWithColor(context, self.scaleColor.CGColor)
        CGContextSetLineWidth(context, self.scaleWidth)
        
        // Middle Marker
        CGContextMoveToPoint(context, margin, middle)
        CGContextAddLineToPoint(context, rect.width - margin, middle)
        
        // Left and Right Markers
        let x = rect.width / 2.0
        let diameter = rect.width - (2 * (self.markerMargin * rect.width))
        let radius = diameter / 2.0
        var leftY = middle
        var rightY = middle
        for var i: UInt = 1; i <= self.markerCount; i++ {
            leftY -= intervalSize
            rightY += intervalSize
            
            CGContextAddEllipseInRect(context, CGRect(x: x - radius, y: leftY - radius, width: diameter, height: diameter))
            CGContextAddEllipseInRect(context, CGRect(x: x - radius, y: rightY - radius, width: diameter, height: diameter))
        }
        
        CGContextStrokePath(context)
        
    }
}
