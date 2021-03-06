//
//  AltitudeScaleView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 24.06.16.
//  Copyright © 2016 Thorsten Kober. All rights reserved.
//

import UIKit

@IBDesignable class AltitudeScaleView: UIView {
    
    
    @IBInspectable var visibleRange: UInt = 1000
    
    
    @IBInspectable var scaleSteps: UInt = 100
    
    
    @IBInspectable var stepsPerLabel = 2
    
    
    @IBInspectable var maxAltitude: UInt = 37000
    
    
    @IBInspectable var scaleColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var labelColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var scaleWidth: CGFloat = 2.0
    
    
    @IBInspectable var fontSize: CGFloat = 0.15
    
    
    @IBInspectable var labelMargin: CGFloat = 0.07
    
    
    @IBInspectable var notchWidth: CGFloat = 18
    
    
    func setAltitude(altitude: UInt) {
        let value = max(0, altitude)
        value
        self._altitude = value
    }
    
    private var _altitude: UInt = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    var scaleImage: UIImage? {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    
    
    override func drawRect(rect: CGRect) {
        if let image = self.scaleImage {
            let scale = UIScreen.mainScreen().scale
            
            let width = rect.width * scale
            let height = rect.height * scale
            let steps = height / CGFloat(self.visibleRange)
            let y = image.size.height - height - steps * CGFloat(self._altitude);
            let sectionFrame = CGRect(x: 0, y: y, width: width, height: height)
            
            let section = CGImageCreateWithImageInRect(image.CGImage, sectionFrame)
            UIImage(CGImage: section!).drawInRect(rect)
        } else {
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.renderScale(rect)
            }
        }
    }
    
    private func renderScale(frame: CGRect) {
        let scale = UIScreen.mainScreen().scale
        
        let frameWidth = frame.width * scale
        let frameHeight = frame.height * scale
        let steps = frameHeight / CGFloat(self.visibleRange)
        
        let imageWidth = frameWidth
        let imageHeight = steps * CGFloat(self.maxAltitude + (self.visibleRange / 2))
        
        UIGraphicsBeginImageContext(CGSize(width: imageWidth, height: imageHeight))
        let context = UIGraphicsGetCurrentContext()
        
        // Scale
        CGContextSetLineWidth(context, self.scaleWidth * scale)
        CGContextSetStrokeColorWithColor(context, self.scaleColor.CGColor)
        
        var value: UInt = 0
        var i = 0
        let labelMargin = self.labelMargin * frameWidth
        while value <= self.maxAltitude {
            let y = imageHeight - (steps * CGFloat((self.visibleRange / 2) + value))
            CGContextMoveToPoint(context, 0, y)
            CGContextAddLineToPoint(context, (self.notchWidth * scale), y)
            
            // Label
            if i % self.stepsPerLabel == 0 {
                let text: NSString = "\(value)"
                let labelAttributes: [String: AnyObject] = [
                    NSFontAttributeName: UIFont.boldSystemFontOfSize(frameWidth * self.fontSize),
                    NSForegroundColorAttributeName: labelColor,
                ]
                
                let boundingBox = text.sizeWithAttributes(labelAttributes)
                text.drawAtPoint(CGPoint(x: frameWidth - boundingBox.width - labelMargin, y: y - (boundingBox.height / 2.0)), withAttributes: labelAttributes)
            }
            
            value += self.scaleSteps
            i += 1
        }
        
        
        CGContextStrokePath(context)
        
        let image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        
        self.scaleImage = image
    }
}
