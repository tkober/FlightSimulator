//
//  AirspeedScaleView.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 22/06/16.
//  Copyright © 2016 Thorsten Kober. All rights reserved.
//

import UIKit


struct TakeOffSpeeds {
    let v1: UInt
    let vr: UInt
    let v2: UInt
}


@IBDesignable class AirspeedScaleView: UIView {

    
    @IBInspectable var visibleRange: UInt = 50
    
    
    @IBInspectable var scaleSteps: UInt = 5
    
    
    @IBInspectable var stepsPerLabel = 2
    
    
    @IBInspectable var maxSpeed: UInt = 505
    
    
    @IBInspectable var scaleColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var labelColor: UIColor = UIColor(colorLiteralRed: 1, green: 1, blue: 1, alpha: 0.7)
    
    
    @IBInspectable var scaleWidth: CGFloat = 2.0
    
    
    @IBInspectable var fontSize: CGFloat = 0.15
    
    
    @IBInspectable var labelMargin: CGFloat = 0.07
    
    
    @IBInspectable var notchWidth: CGFloat = 40
    
    
    @IBInspectable var areaWidth: CGFloat = 20
    
    
    @IBInspectable var markerPointerWidth: CGFloat = 12
    
    
    @IBInspectable var markerHeight: CGFloat = 20
    
    
    @IBInspectable var markerColor: UIColor = UIColor.whiteColor()
    
    
    @IBInspectable var markerLabelColor: UIColor = UIColor(colorLiteralRed: 25.0/255.0, green: 25.0/255.0, blue: 25.0/255.0, alpha: 1)
    
    
    @IBInspectable var markerFontSize: CGFloat = 0.8
    

    func setAirspeed(airspeed: UInt) {
        let value = max(0, airspeed)
        value
        self._airspeed = value
    }
    
    private var _airspeed: UInt = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    func setTakeOffSpeeds(takeOffSpeeds: TakeOffSpeeds?) {
        self._takeOffSpeeds = takeOffSpeeds
    }
    
    private var _takeOffSpeeds: TakeOffSpeeds? {
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
            let y = image.size.height - height - steps * CGFloat(self._airspeed);
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
        let imageHeight = steps * CGFloat(self.maxSpeed + (self.visibleRange / 2))
        
        UIGraphicsBeginImageContext(CGSize(width: imageWidth, height: imageHeight))
        let context = UIGraphicsGetCurrentContext()
        
        // Scale
        CGContextSetLineWidth(context, self.scaleWidth * scale)
        CGContextSetStrokeColorWithColor(context, self.scaleColor.CGColor)
        
        var value: UInt = 0
        var i = 0
        let labelMargin = self.labelMargin * frameWidth
        while value <= self.maxSpeed {
            let y = imageHeight - (steps * CGFloat((self.visibleRange / 2) + value))
            CGContextMoveToPoint(context, imageWidth - (self.areaWidth * scale), y)
            CGContextAddLineToPoint(context, imageWidth - (self.notchWidth * scale), y)
            
            // Label
            if i % self.stepsPerLabel == 0 {
                let text: NSString = "\(value)"
                let labelAttributes: [String: AnyObject] = [
                    NSFontAttributeName: UIFont.boldSystemFontOfSize(frameWidth * self.fontSize),
                    NSForegroundColorAttributeName: labelColor,
                ]
                
                let boundingBox = text.sizeWithAttributes(labelAttributes)
                text.drawAtPoint(CGPoint(x: labelMargin, y: y - (boundingBox.height / 2.0)), withAttributes: labelAttributes)
            }
            
            value += self.scaleSteps
            i += 1
        }
        
        CGContextStrokePath(context)
        
        if let speeds = self._takeOffSpeeds {
            if speeds.v1 == speeds.vr {
                drawTakeOffSpeedMarker(speeds.v1, label: "V1r", context: context, height: imageHeight, width: imageWidth, steps: steps)
            } else {
                drawTakeOffSpeedMarker(speeds.v1, label: "V1", context: context, height: imageHeight, width: imageWidth, steps: steps)
                drawTakeOffSpeedMarker(speeds.vr, label: "Vr", context: context, height: imageHeight, width: imageWidth, steps: steps)
            }
            drawTakeOffSpeedMarker(speeds.v2, label: "V2", context: context, height: imageHeight, width: imageWidth, steps: steps)
        }
    
        let image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        
        self.scaleImage = image
    }
    
    
    private func drawTakeOffSpeedMarker(speed: UInt, label: String, context: CGContextRef?, height: CGFloat, width: CGFloat, steps: CGFloat) {
        let scale = UIScreen.mainScreen().scale
        let markerWidth = self.notchWidth * scale;
        let markerHeight = self.markerHeight * scale;
        let pointerWidth = self.markerPointerWidth * scale;
        
        CGContextSetFillColorWithColor(context, self.markerColor.CGColor)
        CGContextSetStrokeColorWithColor(context, self.markerColor.CGColor)
        CGContextSetLineWidth(context, 1);
        
        let centerY = height - (steps * CGFloat((self.visibleRange / 2) + speed))
        
        CGContextMoveToPoint(context, width - markerWidth, centerY);
        CGContextAddLineToPoint(context, width - markerWidth + pointerWidth, centerY - markerHeight / 2)
        CGContextAddLineToPoint(context, width, centerY - markerHeight / 2);
        CGContextAddLineToPoint(context, width, centerY + markerHeight / 2);
        CGContextAddLineToPoint(context, width - markerWidth + pointerWidth, centerY + markerHeight / 2)
        CGContextAddLineToPoint(context, width - markerWidth, centerY);
        
        CGContextDrawPath(context, CGPathDrawingMode.FillStroke)
        
        let labelAttributes: [String: AnyObject] = [
            NSFontAttributeName: UIFont.boldSystemFontOfSize(markerHeight * self.markerFontSize),
            NSForegroundColorAttributeName: markerLabelColor,
        ]
        
        let boundingBox = label.sizeWithAttributes(labelAttributes)
        label.drawAtPoint(CGPoint(x: width - markerWidth + pointerWidth, y: centerY - (boundingBox.height / 2.0)), withAttributes: labelAttributes)
    }
}
