//
//  zutil.h
//  SimpleMtkWlan
//
//  Created by laobamac (王孝慈) on 2026/3/12.
//  Copyright © 2026 laobamac (王孝慈). All rights reserved.
//

//
//  zutil.h
//  SimpleMtkWlan
//
//  Created by qcwap on 2020/9/4.
//  Copyright © 2020 钟先耀. All rights reserved.
//

#ifndef zutil_h
#define zutil_h

#include <IOKit/IOLib.h>
#include <IOKit/IOTypes.h>

extern "C" {
void *zcalloc(void *opaque, uint items, uint size);

void zcfree(void *opaque, void *ptr);
}

#endif /* zutil_h */
