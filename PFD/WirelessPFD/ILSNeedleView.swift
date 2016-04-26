//
//  ILSNeedleView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 09.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit


@IBDesignable class ILSNeedleView : UIView {
    
    
    @IBInspectable var needleColor: UIColor = UIColor(red: 255/255.0, green: 102/255.0, blue: 255/255.0, alpha: 1)
    
    
    @IBInspectable var needleWidth: CGFloat = 3.0
    
    
    @IBInspectable var margin: CGFloat = 0.2
    
    
    var position: Int {
        get {
            return _position
        }
    }
    
    
    private let POSITIONS = 127
    
    
    private var _position: Int = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    
    func setPosition(position: Int) {
        var value = min(POSITIONS, position)
        value = max(-POSITIONS, value)
        self._position = position
    }
}



@IBDesignable class ILSLocalizerNeedleView : ILSNeedleView {
    
    override func drawRect(rect: CGRect) {
        
        let margin = rect.height * self.margin
        let halfWidth = floor((rect.height - (2 * margin)) * 0.75)
        
        let context = UIGraphicsGetCurrentContext()
        CGContextSetStrokeColorWithColor(context, self.needleColor.CGColor)
        CGContextSetLineWidth(context, self.needleWidth)
        CGContextSetLineCap(context, CGLineCap.Butt)
        
        let pointsPerPosition = rect.width / CGFloat(2 * POSITIONS)
        let x = CGFloat(self.position + POSITIONS) * pointsPerPosition
        let center = CGPointMake(x, rect.height / 2.0)
        
        CGContextMoveToPoint(context, center.x - halfWidth, center.y)
        CGContextAddLineToPoint(context, center.x, margin)
        CGContextAddLineToPoint(context, center.x + halfWidth, center.y)
        CGContextAddLineToPoint(context, center.x, rect.height - margin)
        CGContextAddLineToPoint(context, center.x - halfWidth, center.y)
        CGContextAddLineToPoint(context, center.x, margin)
        CGContextStrokePath(context)
    }
}



@IBDesignable class ILSGlideslopeNeedleView : ILSNeedleView {
    
    override func drawRect(rect: CGRect) {
        
        let margin = rect.width * self.margin
        let halfheight = floor((rect.width - (2 * margin)) * 0.75)
        
        let context = UIGraphicsGetCurrentContext()
        CGContextSetStrokeColorWithColor(context, self.needleColor.CGColor)
        CGContextSetLineWidth(context, self.needleWidth)
        CGContextSetLineCap(context, CGLineCap.Butt)
        
        let pointsPerPosition = rect.height / CGFloat(2 * POSITIONS)
        let y = CGFloat(self.position + POSITIONS) * pointsPerPosition
        let center = CGPointMake(rect.width / 2.0, y)
        
        CGContextMoveToPoint(context, margin, center.y)
        CGContextAddLineToPoint(context, center.x, center.y - halfheight)
        CGContextAddLineToPoint(context, rect.width - margin, center.y)
        CGContextAddLineToPoint(context, center.x, center.y + halfheight)
        CGContextAddLineToPoint(context, margin, center.y)
        CGContextAddLineToPoint(context, center.x, center.y - halfheight)
        CGContextStrokePath(context)
    }
}








