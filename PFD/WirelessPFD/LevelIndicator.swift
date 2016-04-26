//
//  ADIOverlayView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 08.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


@IBDesignable class LevelIndicator : UIView {
    
    
    @IBInspectable var fillColor: UIColor = UIColor.blackColor()
    
    
    @IBInspectable var strokeColor: UIColor = UIColor.whiteColor()
    
    
    @IBInspectable var lineWidth: CGFloat = 1.0
    
    
    @IBInspectable var margin: CGFloat = 0.05
    
    
    @IBInspectable var barWidth: CGFloat = 0.2
    
    
    @IBInspectable var barHeight: CGFloat = 0.03
    
    
    
    override func drawRect(rect: CGRect) {
        
        let context = UIGraphicsGetCurrentContext()
        
        CGContextSetFillColorWithColor(context, self.fillColor.CGColor)
        CGContextSetStrokeColorWithColor(context, self.strokeColor.CGColor)
        CGContextSetLineWidth(context, self.lineWidth)
        
        let levelIndicatorSize = rect.width * self.barHeight
        
        // Center
        let centerRect = CGRectMake((rect.width - levelIndicatorSize) / 2.0, (rect.height - levelIndicatorSize) / 2.0, levelIndicatorSize, levelIndicatorSize)
        CGContextAddRect(context, centerRect)
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
        
        let margin = rect.width * self.margin
        let barWidth = rect.width * self.barWidth
        let top = (rect.height - levelIndicatorSize) / 2.0
        
        // Left
        let left = margin
        CGContextMoveToPoint(context, left, top)
        CGContextAddLineToPoint(context, left + barWidth, top)
        CGContextAddLineToPoint(context, left + barWidth, top + 2*levelIndicatorSize)
        CGContextAddLineToPoint(context, left + barWidth - levelIndicatorSize, top + 2*levelIndicatorSize)
        CGContextAddLineToPoint(context, left + barWidth - levelIndicatorSize, top + levelIndicatorSize)
        CGContextAddLineToPoint(context, left, top + levelIndicatorSize)
        CGContextAddLineToPoint(context, left, top)
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
        
        // Right
        let right = rect.width - margin
        CGContextMoveToPoint(context, right, top)
        CGContextAddLineToPoint(context, right - barWidth, top)
        CGContextAddLineToPoint(context, right - barWidth, top + 2*levelIndicatorSize)
        CGContextAddLineToPoint(context, right - barWidth + levelIndicatorSize, top + 2*levelIndicatorSize)
        CGContextAddLineToPoint(context, right - barWidth + levelIndicatorSize, top + levelIndicatorSize)
        CGContextAddLineToPoint(context, right, top + levelIndicatorSize)
        CGContextAddLineToPoint(context, right, top)
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
    }
    
}
