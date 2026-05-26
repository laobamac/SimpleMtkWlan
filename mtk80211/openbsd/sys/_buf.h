//
//  _buf.h
//  SimpleMtkWlan
//
//  Created by laobamac (王孝慈) on 2026/4/21.
//  Copyright © 2026 laobamac (王孝慈). All rights reserved.
//

//
//  _buf.h
//  SimpleMtkWlan
//
//  Created by 钟先耀 on 2020/1/25.
//  Copyright © 2020 钟先耀. All rights reserved.
//

/*
* Copyright (C) 2020  钟先耀
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef _buf_h
#define _buf_h

/* Macros to clear/set/test flags. */
#define SET(t, f)       (t) |= (f)
#define CLR(t, f)       (t) &= ~(f)
#define ISSET(t, f)     ((t) & (f))

#endif /* _buf_h */
