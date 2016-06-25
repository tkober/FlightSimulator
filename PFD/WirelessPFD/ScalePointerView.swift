//
//  ScalePointerView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 10.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


@IBDesignable class ScalePointerView : UIView {
    
    @IBInspectable var borderColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var fillColor: UIColor = UIColor(colorLiteralRed: 0/255.0, green: 0/255.0, blue: 0/255.0, alpha: 1)
    
    
    @IBInspectable var borderWidth: CGFloat = 2
    
    
    @IBInspectable var pointerHeight: CGFloat = 20
    
    
    @IBInspectable var pointerWidth: CGFloat = 12
}



@IBDesignable class LeftScalePointerView : ScalePointerView {
    
    override func drawRect(rect: CGRect) {
        
        let halfBorderWidth = floor(self.borderWidth / 2.0)
        let adjustedRect = CGRect(x: rect.origin.x + halfBorderWidth, y: rect.origin.y + halfBorderWidth, width: rect.width - halfBorderWidth, height: rect.height - halfBorderWidth)
        
        let context = UIGraphicsGetCurrentContext()
        CGContextSetStrokeColorWithColor(context, self.borderColor.CGColor)
        CGContextSetFillColorWithColor(context, self.fillColor.CGColor)
        CGContextSetLineWidth(context, self.borderWidth)
        CGContextSetLineCap(context, CGLineCap.Round)
        
        CGContextMoveToPoint(context, adjustedRect.origin.x, adjustedRect.origin.y)
        CGContextAddLineToPoint(context, adjustedRect.width - self.pointerWidth, adjustedRect.origin.y)
        CGContextAddLineToPoint(context, adjustedRect.width - self.pointerWidth, (adjustedRect.height - self.pointerHeight) / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.width, adjustedRect.height / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.width - self.pointerWidth, (adjustedRect.height + self.pointerHeight) / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.width - self.pointerWidth, adjustedRect.height)
        CGContextAddLineToPoint(context, adjustedRect.origin.x, adjustedRect.height)
        CGContextAddLineToPoint(context, adjustedRect.origin.x, adjustedRect.origin.y)
  
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
    }
}



@IBDesignable class RightScalePointerView : ScalePointerView {
    
    override func drawRect(rect: CGRect) {
        
        let halfBorderWidth = floor(self.borderWidth / 2.0)
        let adjustedRect = CGRect(x: rect.origin.x + halfBorderWidth, y: rect.origin.y + halfBorderWidth, width: rect.width - halfBorderWidth, height: rect.height - halfBorderWidth)
        
        let context = UIGraphicsGetCurrentContext()
        CGContextSetStrokeColorWithColor(context, self.borderColor.CGColor)
        CGContextSetFillColorWithColor(context, self.fillColor.CGColor)
        CGContextSetLineWidth(context, self.borderWidth)
        CGContextSetLineCap(context, CGLineCap.Round)
        
        CGContextMoveToPoint(context, adjustedRect.origin.x + self.pointerWidth, adjustedRect.origin.y)
        CGContextAddLineToPoint(context, adjustedRect.width, adjustedRect.origin.y)
        CGContextAddLineToPoint(context, adjustedRect.width, adjustedRect.height)
        CGContextAddLineToPoint(context, adjustedRect.origin.x + self.pointerWidth, adjustedRect.height)
        CGContextAddLineToPoint(context, adjustedRect.origin.x + self.pointerWidth, (adjustedRect.height + self.pointerHeight) / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.origin.x, adjustedRect.height / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.origin.x + self.pointerWidth, (adjustedRect.height - self.pointerHeight) / 2.0)
        CGContextAddLineToPoint(context, adjustedRect.origin.x + self.pointerWidth, adjustedRect.origin.y)
        
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
    }
}