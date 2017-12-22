//
//  HLRemoteClient.m
//  HomeLed
//
//  Created by ANDREI VAYAVODA on 04.08.15.
//  Copyright (c) 2015 Voevoda's Incorporated. All rights reserved.
//

#import "HLRemoteClient.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

@interface HLRemoteClient ()

@end

@implementation HLRemoteClient

#pragma mark - Initialization

- (void)dealloc {
    
}

#pragma mark - Public Methods
+ (void)setColor:(UIColor *)color {
    [self sendDictionary:@{@"command":@"Set Color", @"value":[self stringWithColor:color]}];
}

+ (void)setColorsList:(NSArray *)colorsList {
    NSMutableArray *colorsStringsList = [@[] mutableCopy];
    for (NSDictionary *dictionary in colorsList) {
        UIColor *color = dictionary[@"color"];
        [colorsStringsList addObject:[self stringWithColor:color]];
    }
    [self sendDictionary:@{@"command":@"Set Colors List", @"value":colorsStringsList}];
}

+ (void)setAnimationEnabled:(BOOL)enabled speed:(CGFloat)speed toRight:(BOOL)toRight {
    if (!enabled) {
        speed = 0.f;
    }
    [self sendDictionary:@{@"command":@"Linear Animation", @"enabled":@(enabled), @"speed":@(speed), @"toRight":@(toRight)}];
}


+ (void)setAnimationColorsList:(NSArray *)array speed:(CGFloat)speed framesCount:(CGFloat)framesCount {
    NSMutableArray *colorsArray = [NSMutableArray new];
    for (NSDictionary *dictionary in array) {
        NSArray *internalArray = dictionary[@"colorsArray"];
        if (![internalArray isKindOfClass:[NSArray class]]) {
            continue;
        }
        NSMutableArray *internalMutableArray = [NSMutableArray new];
        for (NSDictionary *colorsData in internalArray) {
            UIColor *color = colorsData[@"color"];
            [internalMutableArray addObject:[self stringWithColor:color]];
        }
        [colorsArray addObject:internalMutableArray];
    }
    [self sendDictionary:@{@"command":@"Animated Colors Sets", @"speed":@(speed), @"framesCount":@(framesCount), @"colors-lists-array":colorsArray}];
}

#pragma mark - Private Methods
+ (NSString *)stringWithColor:(UIColor *)color {
    CGFloat red, green, blue;
    [color getRed:&red green:&green blue:&blue alpha:nil];
    return [[NSString stringWithFormat:@"%02x%02x%02x", @(255 * red).integerValue, @(255 * green).integerValue, @(255 * blue).integerValue] uppercaseString];
}

+ (void)sendDictionary:(NSDictionary *)dictionary {
    NSData *bytes = [NSJSONSerialization dataWithJSONObject:dictionary options:0 error:nil];
    NSError *error = nil;
    HLSettings *settings = [HLSettings shared];
    [self sendData:bytes toIpAddress:settings.ipAddress port:settings.port.integerValue error:&error];
}

+ (BOOL)sendData:(const NSData *)data toIpAddress:(NSString *)ipAddress port:(uint16_t)port error:(NSError **)error {
    int sock;
    struct sockaddr_in destination;
    unsigned int echolen;
    int broadcast = 1;
    
    if (!data.length) {
        if (error) {
            *error = [NSError errorWithDomain:@"OXNetworkingDomain" code:-1 userInfo:@{@"description":@"Message must have a positive length"}];
        }
        close(sock);
        return NO;
    }
    
    /* Create the UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        if (error) {
            *error = [NSError errorWithDomain:@"OXNetworkingDomain" code:-1 userInfo:@{@"description":@"Failed to create socket"}];
        }
        close(sock);
        return NO;
    }
    
    /* Construct the server sockaddr_in structure */
    memset(&destination, 0, sizeof(destination));
    
    /* Clear struct */
    destination.sin_family = AF_INET;
    
    /* Internet/IP */
    destination.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    /* IP address */
    destination.sin_port = htons(port);
    
    /* server port */
    setsockopt(sock,
               IPPROTO_IP,
               IP_MULTICAST_IF,
               &destination,
               sizeof(destination));
    const char *cmsg =(const char *)data.bytes;   echolen = data.length;
    
    // this call is what allows broadcast packets to be sent:
    if (setsockopt(sock,
                   SOL_SOCKET,
                   SO_BROADCAST,
                   &broadcast,
                   sizeof broadcast) == -1)
    {
        if (error) {
            *error = [NSError errorWithDomain:@"OXNetworkingDomain" code:-1 userInfo:@{@"description":@"perror: setsockopt (SO_BROADCAST)"}];
        }
        close(sock);
        return NO;
    }
    int sentLength = sendto(sock,
                            cmsg,
                            echolen,
                            0,
                            (struct sockaddr *) &destination,
                            sizeof(destination));
    if (sentLength != echolen)
    {
        if (error) {
            *error = [NSError errorWithDomain:@"OXNetworkingDomain" code:-1 userInfo:@{@"description":@"Mismatch in number of sent bytes"}];
        }
        close(sock);
        return NO;
    }
    
    close(sock);
    return YES;
}

@end
