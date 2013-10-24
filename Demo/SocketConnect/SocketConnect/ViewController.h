//
//  ViewController.h
//  SocketConnect
//
//  Created by Jiangy on 13-10-24.
//  Copyright (c) 2013年 Jiangy. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController

@property (retain, nonatomic) IBOutlet UITextField *tfIP;
@property (retain, nonatomic) IBOutlet UITextField *tfPort;
- (IBAction)onConnect:(id)sender;

@end
