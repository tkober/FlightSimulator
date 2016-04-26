//
//  ADI.swift
//  WirelessPFD
//
//  Created by Thorsten Kober on 08.12.15.
//  Copyright © 2015 Thorsten Kober. All rights reserved.
//

import UIKit



func RADIANS_TO_DEGREES(radians: Double) -> CGFloat { return CGFloat(radians * 180.0 / M_PI) }
func DEGREES_TO_RADIANS(degrees: Double) -> CGFloat { return CGFloat(degrees / 180.0 * M_PI) }


@IBDesignable class AttitudeIndicatorView : UIView {
    
    
    @IBInspectable var skyColor: UIColor = UIColor(red: 27.0/255.0, green: 153.0/255.0, blue: 244.0/255.0, alpha: 1.0)
    
    
    @IBInspectable var groundColor: UIColor = UIColor(red: 128.0/255.0, green: 80.0/255.0, blue: 0.0/255.0, alpha: 1.0)
    
    
    @IBInspectable var horizontColor: UIColor = UIColor.whiteColor()
    
    
    @IBInspectable var horizontWidth: CGFloat = 2.0
    
    
    @IBInspectable var pitchColor: UIColor = UIColor.whiteColor()
    
    
    @IBInspectable var scaleWidth: CGFloat = 2.0
    
    
    @IBInspectable var scaleMargin: CGFloat = 0.3
    
    
    @IBInspectable var halfIndent: CGFloat = 0.13
    
    
    @IBInspectable var quaterIndent: CGFloat = 0.17
    
    
    @IBInspectable var scaleOverlapping: CGFloat = 100 // 20
    
    
    @IBInspectable var heightRatio: CGFloat = 7 // 10
    
    
    @IBInspectable var fontSize: CGFloat = 0.05
    
    
    @IBInspectable var labelMargin: CGFloat = 0.015
    
    
    func setBankAngle(bankAngle: Double) {
        var angle = max(bankAngle, -180)
        angle = min(angle, 180)
        self._bankAngle = angle
    }
    
    var bankAngle: Double {
        get {
            return self._bankAngle
        }
    }
    
    
    private var _bankAngle: Double = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    
    func setPitch(pitch: Double) {
        var angle = max(pitch, -180)
        angle = min(angle, 180)
        self._pitch = angle
    }
    
    
    var pitch: Double {
        get {
            return self._pitch
        }
    }
    
    
    private var _pitch: Double = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    
    var scaleImage: UIImage? {
        didSet {
            self.setNeedsDisplay()
        }
    }
    
    
    func rerender() {
        self.scaleImage = nil
        self.setNeedsDisplay()
    }
    
    
    override func drawRect(rect: CGRect) {
        if let image = self.scaleImage {
            let scale = UIScreen.mainScreen().scale
            
            let totalDegrees = (180 + self.scaleOverlapping) * 2
            let pointsPerDegree = image.size.height / totalDegrees
            
            let y = ((180 + self.scaleOverlapping) + CGFloat(self.pitch)) * pointsPerDegree
            let width = rect.width * scale
            let height = rect.height * scale
            let sectionFrame = CGRect(x: 0, y: y - (height / 2.0), width: width, height: height)
            
            let section = CGImageCreateWithImageInRect(image.CGImage, sectionFrame)
            UIImage(CGImage: section!).drawInRect(rect)
            
            self.layer.transform = CATransform3DMakeRotation(DEGREES_TO_RADIANS(self.bankAngle), 0.0, 0.0, 1.0)
        } else {
            dispatch_async(dispatch_get_main_queue()) { () -> Void in
                self.renderArtificialHorizont(rect)
            }
        }
    }
    
    
    func renderArtificialHorizont(frame: CGRect) {
        let screenScale = UIScreen.mainScreen().scale
        let rect = CGRect(origin: frame.origin, size: CGSize(width: frame.width * screenScale, height: frame.width * screenScale * self.heightRatio))
        UIGraphicsBeginImageContext(rect.size)
        let context = UIGraphicsGetCurrentContext()
        let totalDegrees = (180 + self.scaleOverlapping) * 2
        let pointsPerDegree = rect.height / totalDegrees
        
        // Ground Overlapping
        var section = CGRectMake(0, 0, rect.width, pointsPerDegree * self.scaleOverlapping)
        CGContextAddRect(context, section)
        CGContextSetFillColorWithColor(context, self.groundColor.CGColor)
        CGContextFillPath(context)
        
        // Sky
        section = CGRectMake(0, pointsPerDegree * self.scaleOverlapping, rect.width, pointsPerDegree * 180)
        CGContextAddRect(context, section)
        CGContextSetFillColorWithColor(context, self.skyColor.CGColor)
        CGContextFillPath(context)
        
        // Ground
        section = CGRectMake(0, pointsPerDegree * (180 + self.scaleOverlapping), rect.width, pointsPerDegree * 180)
        CGContextAddRect(context, section)
        CGContextSetFillColorWithColor(context, self.groundColor.CGColor)
        CGContextFillPath(context)
        
        // Sky Overlapping
        section = CGRectMake(0, pointsPerDegree * (360 + self.scaleOverlapping), rect.width, pointsPerDegree * self.scaleOverlapping)
        CGContextAddRect(context, section)
        CGContextSetFillColorWithColor(context, self.skyColor.CGColor)
        CGContextFillPath(context)
        
        // Horizont(s)
        CGContextSetLineWidth(context, self.horizontWidth * screenScale)
        CGContextSetStrokeColorWithColor(context, self.horizontColor.CGColor)
        
        CGContextMoveToPoint(context, 0, pointsPerDegree * self.scaleOverlapping)
        CGContextAddLineToPoint(context, rect.width, pointsPerDegree * self.scaleOverlapping)
        
        CGContextMoveToPoint(context, 0, pointsPerDegree * (180 + self.scaleOverlapping))
        CGContextAddLineToPoint(context, rect.width, pointsPerDegree * (180 + self.scaleOverlapping))
        
        CGContextMoveToPoint(context, 0, pointsPerDegree * (360 + self.scaleOverlapping))
        CGContextAddLineToPoint(context, rect.width, pointsPerDegree * (360 + self.scaleOverlapping))
        
        CGContextStrokePath(context)
        
        // Scale
        CGContextSetStrokeColorWithColor(context, self.pitchColor.CGColor)
        CGContextSetLineWidth(context, self.scaleWidth * screenScale)
        
                CGContextBeginPath(context)
        
        let left = rect.width * self.scaleMargin
        let right = rect.width - rect.width * self.scaleMargin
        let degrees = Int(180 + self.scaleOverlapping)
        let middle = ceil(CGFloat(degrees) * pointsPerDegree)
        
        var indent = 1
        for var i: CGFloat = 2.5; i <= CGFloat(degrees); i = i + 2.5 {
            let indentWith: CGFloat
            switch indent {
                
            case 1, 3:
                indentWith = self.quaterIndent * rect.width
                
            case 2:
                indentWith = self.halfIndent * rect.width
                
            default:
                indentWith = 0
                
            }
            
            let y = ceil(i * pointsPerDegree)
            
            CGContextMoveToPoint(context, left + indentWith, middle - y)
            CGContextAddLineToPoint(context, right - indentWith, middle - y)
            
            CGContextMoveToPoint(context, left + indentWith, middle + y)
            CGContextAddLineToPoint(context, right - indentWith, middle + y)
            
            
            indent = (indent + 1) % 4
        }
        CGContextStrokePath(context)

        // Labels
        for var i = 0; i <= degrees; i = i + 10 {
            let j = i % 180
            if j != 0 {
                let text: NSString = "\(j)"
                let labelAttributes: [String: AnyObject] = [
                    NSFontAttributeName: UIFont.boldSystemFontOfSize(rect.width * self.fontSize),
                    NSForegroundColorAttributeName: UIColor.whiteColor(),
                ]
                
                let boundingBox = text.sizeWithAttributes(labelAttributes)
                let margin = self.labelMargin * rect.width
                
                let upperY = (middle - CGFloat(i) * pointsPerDegree) - boundingBox.height / 2.0
                text.drawAtPoint(CGPoint(x: left - boundingBox.width - margin, y: upperY), withAttributes: labelAttributes)
                text.drawAtPoint(CGPoint(x: right + margin, y: upperY), withAttributes: labelAttributes)
                
                let lowerY = (middle + CGFloat(i) * pointsPerDegree) - boundingBox.height / 2.0
                text.drawAtPoint(CGPoint(x: left - boundingBox.width - margin, y: lowerY), withAttributes: labelAttributes)
                text.drawAtPoint(CGPoint(x: right + margin, y: lowerY), withAttributes: labelAttributes)
            }
        }
        
        let image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        
//        let path = "\(NSHomeDirectory())/pfd.png"
//        print("path -> \(path)")
//        UIImagePNGRepresentation(image)?.writeToFile(path, atomically: true)
        
        self.scaleImage = image
    }
    
}