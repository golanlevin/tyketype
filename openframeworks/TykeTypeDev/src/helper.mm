//
//  helper.m
//  TykeTypeDev
//
//  Created by GL on 11/13/21.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>

void CallSomeCocoaFunction() {
   // Do Cocoa stuff here
	
	[NSApp setPresentationOptions: NSApplicationPresentationHideMenuBar | NSApplicationPresentationHideDock | NSApplicationPresentationDisableAppleMenu | NSApplicationPresentationDisableHideApplication | NSApplicationPresentationDisableProcessSwitching ];

}
