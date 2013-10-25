//
//  ViewController.mm
//  SocketConnect
//
//  Created by Jiangy on 13-10-24.
//  Copyright (c) 2013年 Jiangy. All rights reserved.
//

#import "ViewController.h"
#include "AppConnect.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_tfIP release];
    [_tfPort release];
    [_btnTest release];
    [super dealloc];
}

- (IBAction)onConnect:(id)sender {
    std::string dstIP([_tfIP.text UTF8String]);
    int dstPost = [_tfPort.text integerValue];
    AppConnect::client()->startServer(dstIP, dstPost);
    [_btnTest setEnabled:YES];
}

- (IBAction)onTest:(id)sender {
    AppConnect::server()->sendTest("SocketConnect");
}

@end
