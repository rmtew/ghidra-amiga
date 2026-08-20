€ˆEXEC_INITIALIZERS_H€EXEC_INITIALIZERS_H€OFFSET(structName,structEntry) \
(&(((ƒstructName*)0)->structEntry))€INITBYTE(offset,value) 0xe000,(‰)(offset),(‰)((value)<<8)€INITWORD(offset,value) 0xd000,(‰)(offset),(‰)(value)€INITLONG(offset,value) 0xc000,(‰)(offset),\
(‰)((value)>>16),\
(‰)((value)&0xffff)€INITSTRUCT(size,offset,value,count) \
(‰)(0xc000|(size<<12)|(count<<8)|\
((‰)((offset)>>16)),\
((‰)(offset))&0xffff)‡