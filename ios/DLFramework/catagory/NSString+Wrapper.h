//
//  NSString+Wrapper.h
//  MeiDi
//
//  Created by Donal on 14-1-3.
//  Copyright (c) 2014年 vikaa. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (Wrapper)

/**  Return the char value at the specified index. */
- (unichar) charAt:(int)index;

/**
 * Compares two strings lexicographically.
 * the value 0 if the argument string is equal to this string;
 * a value less than 0 if this string is lexicographically less than the string argument;
 * and a value greater than 0 if this string is lexicographically greater than the string argument.
 */
- (int) compareTo:(NSString*) anotherString;

- (int) compareToIgnoreCase:(NSString*) str;

- (BOOL) contains:(NSString*) str;

- (BOOL) startsWith:(NSString*)prefix;

- (BOOL) endsWith:(NSString*)suffix;

- (BOOL) equals:(NSString*) anotherString;

- (BOOL) equalsIgnoreCase:(NSString*) anotherString;

- (int) indexOfChar:(unichar)ch;

- (int) indexOfChar:(unichar)ch fromIndex:(int)index;

- (int) indexOfString:(NSString*)str;

- (int) indexOfString:(NSString*)str fromIndex:(int)index;

- (int) lastIndexOfChar:(unichar)ch;

- (int) lastIndexOfChar:(unichar)ch fromIndex:(int)index;

- (int) lastIndexOfString:(NSString*)str;

- (int) lastIndexOfString:(NSString*)str fromIndex:(int)index;

- (NSString *) substringFromIndex:(int)beginIndex toIndex:(int)endIndex;

- (NSString *) toLowerCase;

- (NSString *) toUpperCase;

- (NSString *) trim;

- (NSString *) replaceAll:(NSString*)origin with:(NSString*)replacement;

- (NSArray *) split:(NSString*) separator;

@end
