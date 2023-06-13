
_demosched:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:
	return (foo(a-1)+foo(a-2));
}


int main(void)
{
   0:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   4:	83 e4 f0             	and    $0xfffffff0,%esp
	int start, end, runtime; // variables for tracking clock ticks of CPU BOUND function
	int priority; // priority variables for the children

	int pPriority[NUM_CHILDREN];
	int pRuntime[NUM_CHILDREN];
	int avg[8]={0};
   7:	31 d2                	xor    %edx,%edx
   9:	31 c0                	xor    %eax,%eax
{
   b:	ff 71 fc             	push   -0x4(%ecx)
   e:	55                   	push   %ebp
   f:	89 e5                	mov    %esp,%ebp
  11:	53                   	push   %ebx
  12:	51                   	push   %ecx
  13:	81 ec 30 01 00 00    	sub    $0x130,%esp
	int avg[8]={0};
  19:	89 94 05 d8 fe ff ff 	mov    %edx,-0x128(%ebp,%eax,1)
  20:	83 c0 04             	add    $0x4,%eax
  23:	83 f8 20             	cmp    $0x20,%eax
  26:	72 f1                	jb     19 <main+0x19>
	int fds[2];
	pipe(fds);
  28:	83 ec 0c             	sub    $0xc,%esp
  2b:	8d 85 d0 fe ff ff    	lea    -0x130(%ebp),%eax
	
	for(int i=0;i<NUM_CHILDREN;i++)
  31:	31 db                	xor    %ebx,%ebx
	pipe(fds);
  33:	50                   	push   %eax
  34:	e8 5a 06 00 00       	call   693 <pipe>
  39:	83 c4 10             	add    $0x10,%esp
  3c:	eb 10                	jmp    4e <main+0x4e>
  3e:	66 90                	xchg   %ax,%ax
			printf(1,"ERROR fork could not create child\n");
			exit();
			return -1;
		}
		if(f!=0){} // do nothing
		if(f==0)
  40:	74 28                	je     6a <main+0x6a>
	for(int i=0;i<NUM_CHILDREN;i++)
  42:	83 c3 01             	add    $0x1,%ebx
  45:	83 fb 20             	cmp    $0x20,%ebx
  48:	0f 84 82 00 00 00    	je     d0 <main+0xd0>
		f = fork();
  4e:	e8 28 06 00 00       	call   67b <fork>
		if(f<0)
  53:	85 c0                	test   %eax,%eax
  55:	79 e9                	jns    40 <main+0x40>
			printf(1,"ERROR fork could not create child\n");
  57:	52                   	push   %edx
  58:	52                   	push   %edx
  59:	68 18 0b 00 00       	push   $0xb18
  5e:	6a 01                	push   $0x1
  60:	e8 8b 07 00 00       	call   7f0 <printf>
			exit();
  65:	e8 19 06 00 00       	call   683 <exit>
		{	
			priority = i%8;
			setprio(priority);
  6a:	83 ec 0c             	sub    $0xc,%esp
			priority = i%8;
  6d:	83 e3 07             	and    $0x7,%ebx
			setprio(priority);
  70:	53                   	push   %ebx
			priority = i%8;
  71:	89 9d 78 ff ff ff    	mov    %ebx,-0x88(%ebp)
			setprio(priority);
  77:	e8 af 06 00 00       	call   72b <setprio>
			start = uptime();
  7c:	e8 9a 06 00 00       	call   71b <uptime>
			foo(TEST_PARAM);
  81:	c7 04 24 23 00 00 00 	movl   $0x23,(%esp)
			start = uptime();
  88:	89 c3                	mov    %eax,%ebx
			foo(TEST_PARAM);
  8a:	e8 f1 00 00 00       	call   180 <foo>
  8f:	83 c4 10             	add    $0x10,%esp
			end = uptime();
  92:	e8 84 06 00 00       	call   71b <uptime>
			runtime = end -start;
  97:	29 d8                	sub    %ebx,%eax
  99:	89 85 f8 fe ff ff    	mov    %eax,-0x108(%ebp)

			write(fds[1],&priority,1);
  9f:	50                   	push   %eax
  a0:	8d 85 78 ff ff ff    	lea    -0x88(%ebp),%eax
  a6:	6a 01                	push   $0x1
  a8:	50                   	push   %eax
  a9:	ff b5 d4 fe ff ff    	push   -0x12c(%ebp)
  af:	e8 ef 05 00 00       	call   6a3 <write>
			write(fds[1],&runtime,1);
  b4:	83 c4 0c             	add    $0xc,%esp
  b7:	8d 85 f8 fe ff ff    	lea    -0x108(%ebp),%eax
  bd:	6a 01                	push   $0x1
  bf:	50                   	push   %eax
  c0:	ff b5 d4 fe ff ff    	push   -0x12c(%ebp)
  c6:	e8 d8 05 00 00       	call   6a3 <write>
			//printf(1,"runtime:%d prio:%d\n",runtime,priority); // debug
			exit();
  cb:	e8 b3 05 00 00       	call   683 <exit>
  d0:	31 db                	xor    %ebx,%ebx
  d2:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi

	}
	for(int i=0;i<NUM_CHILDREN;i++)
	{
		//piping
		wait();
  d8:	e8 ae 05 00 00       	call   68b <wait>
		read(fds[0],&pPriority[i],1);
  dd:	8d 85 f8 fe ff ff    	lea    -0x108(%ebp),%eax
  e3:	83 ec 04             	sub    $0x4,%esp
  e6:	01 d8                	add    %ebx,%eax
  e8:	6a 01                	push   $0x1
  ea:	50                   	push   %eax
  eb:	ff b5 d0 fe ff ff    	push   -0x130(%ebp)
  f1:	e8 a5 05 00 00       	call   69b <read>
		read(fds[0],&pRuntime[i],1);
  f6:	8d 85 78 ff ff ff    	lea    -0x88(%ebp),%eax
  fc:	83 c4 0c             	add    $0xc,%esp
  ff:	01 d8                	add    %ebx,%eax
 101:	6a 01                	push   $0x1
	for(int i=0;i<NUM_CHILDREN;i++)
 103:	83 c3 04             	add    $0x4,%ebx
		read(fds[0],&pRuntime[i],1);
 106:	50                   	push   %eax
 107:	ff b5 d0 fe ff ff    	push   -0x130(%ebp)
 10d:	e8 89 05 00 00       	call   69b <read>
	for(int i=0;i<NUM_CHILDREN;i++)
 112:	83 c4 10             	add    $0x10,%esp
 115:	81 fb 80 00 00 00    	cmp    $0x80,%ebx
 11b:	75 bb                	jne    d8 <main+0xd8>
	}
	int num = NUM_CHILDREN/8;
	for(int i=0;i<NUM_CHILDREN;i++)
 11d:	31 c0                	xor    %eax,%eax
 11f:	90                   	nop
	{
		avg[pPriority[i]] += pRuntime[i];
 120:	8b 94 85 f8 fe ff ff 	mov    -0x108(%ebp,%eax,4),%edx
 127:	8b 8c 85 78 ff ff ff 	mov    -0x88(%ebp,%eax,4),%ecx
	for(int i=0;i<NUM_CHILDREN;i++)
 12e:	83 c0 01             	add    $0x1,%eax
		avg[pPriority[i]] += pRuntime[i];
 131:	01 8c 95 d8 fe ff ff 	add    %ecx,-0x128(%ebp,%edx,4)
	for(int i=0;i<NUM_CHILDREN;i++)
 138:	83 f8 20             	cmp    $0x20,%eax
 13b:	75 e3                	jne    120 <main+0x120>
	}

	for(int i=0;i<8;i++)
 13d:	31 db                	xor    %ebx,%ebx
 13f:	90                   	nop
	{
		avg[i]/=num;
 140:	8b 94 9d d8 fe ff ff 	mov    -0x128(%ebp,%ebx,4),%edx
 147:	85 d2                	test   %edx,%edx
 149:	8d 42 03             	lea    0x3(%edx),%eax
 14c:	0f 49 c2             	cmovns %edx,%eax
 14f:	c1 f8 02             	sar    $0x2,%eax
		printf(1,"Priority: %d - Runtime(average): %d\n",i,avg[i]);
 152:	50                   	push   %eax
 153:	53                   	push   %ebx
 154:	68 3c 0b 00 00       	push   $0xb3c
 159:	6a 01                	push   $0x1
		avg[i]/=num;
 15b:	89 84 9d d8 fe ff ff 	mov    %eax,-0x128(%ebp,%ebx,4)
	for(int i=0;i<8;i++)
 162:	83 c3 01             	add    $0x1,%ebx
		printf(1,"Priority: %d - Runtime(average): %d\n",i,avg[i]);
 165:	e8 86 06 00 00       	call   7f0 <printf>
	for(int i=0;i<8;i++)
 16a:	83 c4 10             	add    $0x10,%esp
 16d:	83 fb 08             	cmp    $0x8,%ebx
 170:	75 ce                	jne    140 <main+0x140>
	}

	exit();
 172:	e8 0c 05 00 00       	call   683 <exit>
 177:	66 90                	xchg   %ax,%ax
 179:	66 90                	xchg   %ax,%ax
 17b:	66 90                	xchg   %ax,%ax
 17d:	66 90                	xchg   %ax,%ax
 17f:	90                   	nop

00000180 <foo>:
{
 180:	55                   	push   %ebp
 181:	89 e5                	mov    %esp,%ebp
 183:	57                   	push   %edi
 184:	56                   	push   %esi
 185:	53                   	push   %ebx
 186:	83 ec 5c             	sub    $0x5c,%esp
 189:	8b 45 08             	mov    0x8(%ebp),%eax
	if(a==0||a==1)
 18c:	c7 45 bc 00 00 00 00 	movl   $0x0,-0x44(%ebp)
 193:	83 e8 01             	sub    $0x1,%eax
 196:	83 7d 08 01          	cmpl   $0x1,0x8(%ebp)
 19a:	89 45 d0             	mov    %eax,-0x30(%ebp)
 19d:	0f 86 82 02 00 00    	jbe    425 <foo+0x2a5>
	return (foo(a-1)+foo(a-2));
 1a3:	8b 45 d0             	mov    -0x30(%ebp),%eax
	if(a==0||a==1)
 1a6:	83 f8 01             	cmp    $0x1,%eax
 1a9:	0f 86 58 02 00 00    	jbe    407 <foo+0x287>
 1af:	8b 45 d0             	mov    -0x30(%ebp),%eax
 1b2:	c7 45 b8 00 00 00 00 	movl   $0x0,-0x48(%ebp)
	return (foo(a-1)+foo(a-2));
 1b9:	83 e8 01             	sub    $0x1,%eax
 1bc:	89 45 ac             	mov    %eax,-0x54(%ebp)
 1bf:	89 45 cc             	mov    %eax,-0x34(%ebp)
 1c2:	8b 45 cc             	mov    -0x34(%ebp),%eax
	if(a==0||a==1)
 1c5:	83 f8 01             	cmp    $0x1,%eax
 1c8:	0f 84 00 02 00 00    	je     3ce <foo+0x24e>
 1ce:	8b 45 cc             	mov    -0x34(%ebp),%eax
 1d1:	c7 45 b4 00 00 00 00 	movl   $0x0,-0x4c(%ebp)
	return (foo(a-1)+foo(a-2));
 1d8:	83 e8 01             	sub    $0x1,%eax
 1db:	89 45 a8             	mov    %eax,-0x58(%ebp)
 1de:	89 45 c4             	mov    %eax,-0x3c(%ebp)
 1e1:	8b 45 c4             	mov    -0x3c(%ebp),%eax
	if(a==0||a==1)
 1e4:	83 f8 01             	cmp    $0x1,%eax
 1e7:	0f 84 a8 01 00 00    	je     395 <foo+0x215>
 1ed:	8b 45 c4             	mov    -0x3c(%ebp),%eax
 1f0:	c7 45 b0 00 00 00 00 	movl   $0x0,-0x50(%ebp)
	return (foo(a-1)+foo(a-2));
 1f7:	83 e8 01             	sub    $0x1,%eax
 1fa:	89 45 a0             	mov    %eax,-0x60(%ebp)
 1fd:	89 45 c8             	mov    %eax,-0x38(%ebp)
 200:	8b 45 c8             	mov    -0x38(%ebp),%eax
	if(a==0||a==1)
 203:	83 f8 01             	cmp    $0x1,%eax
 206:	0f 84 50 01 00 00    	je     35c <foo+0x1dc>
 20c:	8b 45 c8             	mov    -0x38(%ebp),%eax
 20f:	c7 45 dc 00 00 00 00 	movl   $0x0,-0x24(%ebp)
 216:	83 e8 01             	sub    $0x1,%eax
 219:	89 45 a4             	mov    %eax,-0x5c(%ebp)
 21c:	89 c1                	mov    %eax,%ecx
 21e:	c7 45 e4 00 00 00 00 	movl   $0x0,-0x1c(%ebp)
	return (foo(a-1)+foo(a-2));
 225:	89 c8                	mov    %ecx,%eax
	if(a==0||a==1)
 227:	8d 51 fd             	lea    -0x3(%ecx),%edx
 22a:	83 f9 01             	cmp    $0x1,%ecx
 22d:	0f 84 f5 00 00 00    	je     328 <foo+0x1a8>
 233:	89 4d d8             	mov    %ecx,-0x28(%ebp)
 236:	89 d6                	mov    %edx,%esi
	return (foo(a-1)+foo(a-2));
 238:	8d 46 02             	lea    0x2(%esi),%eax
	if(a==0||a==1)
 23b:	83 f8 01             	cmp    $0x1,%eax
 23e:	0f 84 af 00 00 00    	je     2f3 <foo+0x173>
 244:	89 75 c0             	mov    %esi,-0x40(%ebp)
 247:	8d 4e ff             	lea    -0x1(%esi),%ecx
 24a:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%ebp)
 251:	89 75 d4             	mov    %esi,-0x2c(%ebp)
 254:	8b 5d c0             	mov    -0x40(%ebp),%ebx
 257:	8d 41 02             	lea    0x2(%ecx),%eax
 25a:	31 f6                	xor    %esi,%esi
 25c:	83 f9 ff             	cmp    $0xffffffff,%ecx
 25f:	74 67                	je     2c8 <foo+0x148>
	return (foo(a-1)+foo(a-2));
 261:	89 df                	mov    %ebx,%edi
	if(a==0||a==1)
 263:	31 d2                	xor    %edx,%edx
 265:	83 fb 01             	cmp    $0x1,%ebx
 268:	76 4a                	jbe    2b4 <foo+0x134>
 26a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
	return (foo(a-1)+foo(a-2));
 270:	83 ec 0c             	sub    $0xc,%esp
 273:	8d 47 ff             	lea    -0x1(%edi),%eax
 276:	89 4d 98             	mov    %ecx,-0x68(%ebp)
 279:	83 ef 02             	sub    $0x2,%edi
 27c:	50                   	push   %eax
 27d:	89 55 9c             	mov    %edx,-0x64(%ebp)
 280:	e8 fb fe ff ff       	call   180 <foo>
 285:	8b 55 9c             	mov    -0x64(%ebp),%edx
 288:	83 c4 10             	add    $0x10,%esp
	if(a==0||a==1)
 28b:	8b 4d 98             	mov    -0x68(%ebp),%ecx
 28e:	01 c2                	add    %eax,%edx
 290:	83 ff 01             	cmp    $0x1,%edi
 293:	77 db                	ja     270 <foo+0xf0>
	return (foo(a-1)+foo(a-2));
 295:	89 df                	mov    %ebx,%edi
 297:	8d 43 fe             	lea    -0x2(%ebx),%eax
	if(a==0||a==1)
 29a:	83 eb 01             	sub    $0x1,%ebx
	return (foo(a-1)+foo(a-2));
 29d:	83 e7 01             	and    $0x1,%edi
 2a0:	01 d7                	add    %edx,%edi
		return a;
 2a2:	01 fe                	add    %edi,%esi
	if(a==0||a==1)
 2a4:	83 fb 01             	cmp    $0x1,%ebx
 2a7:	76 18                	jbe    2c1 <foo+0x141>
 2a9:	89 c3                	mov    %eax,%ebx
 2ab:	31 d2                	xor    %edx,%edx
	return (foo(a-1)+foo(a-2));
 2ad:	89 df                	mov    %ebx,%edi
	if(a==0||a==1)
 2af:	83 fb 01             	cmp    $0x1,%ebx
 2b2:	77 bc                	ja     270 <foo+0xf0>
 2b4:	8d 43 fe             	lea    -0x2(%ebx),%eax
 2b7:	83 eb 01             	sub    $0x1,%ebx
 2ba:	01 fe                	add    %edi,%esi
 2bc:	83 fb 01             	cmp    $0x1,%ebx
 2bf:	77 e8                	ja     2a9 <foo+0x129>
	return (foo(a-1)+foo(a-2));
 2c1:	89 c8                	mov    %ecx,%eax
 2c3:	83 e0 01             	and    $0x1,%eax
 2c6:	01 f0                	add    %esi,%eax
	if(a==0||a==1)
 2c8:	01 45 e0             	add    %eax,-0x20(%ebp)
 2cb:	8d 41 fe             	lea    -0x2(%ecx),%eax
 2ce:	83 c1 01             	add    $0x1,%ecx
 2d1:	83 6d c0 02          	subl   $0x2,-0x40(%ebp)
 2d5:	83 f9 01             	cmp    $0x1,%ecx
 2d8:	76 0e                	jbe    2e8 <foo+0x168>
 2da:	89 c1                	mov    %eax,%ecx
 2dc:	e9 73 ff ff ff       	jmp    254 <foo+0xd4>
 2e1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
	return (foo(a-1)+foo(a-2));
 2e8:	8b 75 d4             	mov    -0x2c(%ebp),%esi
 2eb:	89 f0                	mov    %esi,%eax
 2ed:	83 e0 01             	and    $0x1,%eax
 2f0:	03 45 e0             	add    -0x20(%ebp),%eax
	if(a==0||a==1)
 2f3:	01 45 e4             	add    %eax,-0x1c(%ebp)
 2f6:	8d 46 fe             	lea    -0x2(%esi),%eax
 2f9:	83 c6 01             	add    $0x1,%esi
 2fc:	83 fe 01             	cmp    $0x1,%esi
 2ff:	76 07                	jbe    308 <foo+0x188>
 301:	89 c6                	mov    %eax,%esi
 303:	e9 30 ff ff ff       	jmp    238 <foo+0xb8>
 308:	8b 4d d8             	mov    -0x28(%ebp),%ecx
	return (foo(a-1)+foo(a-2));
 30b:	89 c8                	mov    %ecx,%eax
 30d:	8d 51 fe             	lea    -0x2(%ecx),%edx
 310:	83 e0 01             	and    $0x1,%eax
 313:	03 45 e4             	add    -0x1c(%ebp),%eax
	if(a==0||a==1)
 316:	01 45 dc             	add    %eax,-0x24(%ebp)
 319:	8d 41 ff             	lea    -0x1(%ecx),%eax
 31c:	83 f8 01             	cmp    $0x1,%eax
 31f:	76 17                	jbe    338 <foo+0x1b8>
 321:	89 d1                	mov    %edx,%ecx
 323:	e9 f6 fe ff ff       	jmp    21e <foo+0x9e>
 328:	01 45 dc             	add    %eax,-0x24(%ebp)
 32b:	8d 41 ff             	lea    -0x1(%ecx),%eax
 32e:	ba ff ff ff ff       	mov    $0xffffffff,%edx
 333:	83 f8 01             	cmp    $0x1,%eax
 336:	77 e9                	ja     321 <foo+0x1a1>
 338:	8b 45 c8             	mov    -0x38(%ebp),%eax
 33b:	8d 50 fe             	lea    -0x2(%eax),%edx
	return (foo(a-1)+foo(a-2));
 33e:	83 e0 01             	and    $0x1,%eax
 341:	03 45 dc             	add    -0x24(%ebp),%eax
	if(a==0||a==1)
 344:	01 45 b0             	add    %eax,-0x50(%ebp)
 347:	83 7d a4 01          	cmpl   $0x1,-0x5c(%ebp)
 34b:	76 24                	jbe    371 <foo+0x1f1>
 34d:	89 55 c8             	mov    %edx,-0x38(%ebp)
	return (foo(a-1)+foo(a-2));
 350:	8b 45 c8             	mov    -0x38(%ebp),%eax
	if(a==0||a==1)
 353:	83 f8 01             	cmp    $0x1,%eax
 356:	0f 85 b0 fe ff ff    	jne    20c <foo+0x8c>
 35c:	01 45 b0             	add    %eax,-0x50(%ebp)
 35f:	ba ff ff ff ff       	mov    $0xffffffff,%edx
 364:	c7 45 a4 00 00 00 00 	movl   $0x0,-0x5c(%ebp)
 36b:	83 7d a4 01          	cmpl   $0x1,-0x5c(%ebp)
 36f:	77 dc                	ja     34d <foo+0x1cd>
 371:	8b 45 c4             	mov    -0x3c(%ebp),%eax
 374:	8d 50 fe             	lea    -0x2(%eax),%edx
	return (foo(a-1)+foo(a-2));
 377:	83 e0 01             	and    $0x1,%eax
 37a:	03 45 b0             	add    -0x50(%ebp),%eax
	if(a==0||a==1)
 37d:	01 45 b4             	add    %eax,-0x4c(%ebp)
 380:	83 7d a0 01          	cmpl   $0x1,-0x60(%ebp)
 384:	76 24                	jbe    3aa <foo+0x22a>
 386:	89 55 c4             	mov    %edx,-0x3c(%ebp)
	return (foo(a-1)+foo(a-2));
 389:	8b 45 c4             	mov    -0x3c(%ebp),%eax
	if(a==0||a==1)
 38c:	83 f8 01             	cmp    $0x1,%eax
 38f:	0f 85 58 fe ff ff    	jne    1ed <foo+0x6d>
 395:	01 45 b4             	add    %eax,-0x4c(%ebp)
 398:	ba ff ff ff ff       	mov    $0xffffffff,%edx
 39d:	c7 45 a0 00 00 00 00 	movl   $0x0,-0x60(%ebp)
 3a4:	83 7d a0 01          	cmpl   $0x1,-0x60(%ebp)
 3a8:	77 dc                	ja     386 <foo+0x206>
 3aa:	8b 45 cc             	mov    -0x34(%ebp),%eax
 3ad:	8d 50 fe             	lea    -0x2(%eax),%edx
	return (foo(a-1)+foo(a-2));
 3b0:	83 e0 01             	and    $0x1,%eax
 3b3:	03 45 b4             	add    -0x4c(%ebp),%eax
	if(a==0||a==1)
 3b6:	01 45 b8             	add    %eax,-0x48(%ebp)
 3b9:	83 7d a8 01          	cmpl   $0x1,-0x58(%ebp)
 3bd:	76 24                	jbe    3e3 <foo+0x263>
 3bf:	89 55 cc             	mov    %edx,-0x34(%ebp)
	return (foo(a-1)+foo(a-2));
 3c2:	8b 45 cc             	mov    -0x34(%ebp),%eax
	if(a==0||a==1)
 3c5:	83 f8 01             	cmp    $0x1,%eax
 3c8:	0f 85 00 fe ff ff    	jne    1ce <foo+0x4e>
 3ce:	01 45 b8             	add    %eax,-0x48(%ebp)
 3d1:	ba ff ff ff ff       	mov    $0xffffffff,%edx
 3d6:	c7 45 a8 00 00 00 00 	movl   $0x0,-0x58(%ebp)
 3dd:	83 7d a8 01          	cmpl   $0x1,-0x58(%ebp)
 3e1:	77 dc                	ja     3bf <foo+0x23f>
 3e3:	8b 45 d0             	mov    -0x30(%ebp),%eax
 3e6:	8d 50 fe             	lea    -0x2(%eax),%edx
	return (foo(a-1)+foo(a-2));
 3e9:	83 e0 01             	and    $0x1,%eax
 3ec:	03 45 b8             	add    -0x48(%ebp),%eax
	if(a==0||a==1)
 3ef:	01 45 bc             	add    %eax,-0x44(%ebp)
 3f2:	83 7d ac 01          	cmpl   $0x1,-0x54(%ebp)
 3f6:	76 21                	jbe    419 <foo+0x299>
 3f8:	89 55 d0             	mov    %edx,-0x30(%ebp)
	return (foo(a-1)+foo(a-2));
 3fb:	8b 45 d0             	mov    -0x30(%ebp),%eax
	if(a==0||a==1)
 3fe:	83 f8 01             	cmp    $0x1,%eax
 401:	0f 87 a8 fd ff ff    	ja     1af <foo+0x2f>
 407:	8d 78 ff             	lea    -0x1(%eax),%edi
 40a:	01 45 bc             	add    %eax,-0x44(%ebp)
 40d:	8d 50 fe             	lea    -0x2(%eax),%edx
 410:	89 7d ac             	mov    %edi,-0x54(%ebp)
 413:	83 7d ac 01          	cmpl   $0x1,-0x54(%ebp)
 417:	77 df                	ja     3f8 <foo+0x278>
	return (foo(a-1)+foo(a-2));
 419:	8b 45 08             	mov    0x8(%ebp),%eax
 41c:	83 e0 01             	and    $0x1,%eax
 41f:	03 45 bc             	add    -0x44(%ebp),%eax
 422:	89 45 08             	mov    %eax,0x8(%ebp)
}
 425:	8b 45 08             	mov    0x8(%ebp),%eax
 428:	8d 65 f4             	lea    -0xc(%ebp),%esp
 42b:	5b                   	pop    %ebx
 42c:	5e                   	pop    %esi
 42d:	5f                   	pop    %edi
 42e:	5d                   	pop    %ebp
 42f:	c3                   	ret    

00000430 <strcpy>:
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
 430:	55                   	push   %ebp
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
 431:	31 c0                	xor    %eax,%eax
{
 433:	89 e5                	mov    %esp,%ebp
 435:	53                   	push   %ebx
 436:	8b 4d 08             	mov    0x8(%ebp),%ecx
 439:	8b 5d 0c             	mov    0xc(%ebp),%ebx
 43c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  while((*s++ = *t++) != 0)
 440:	0f b6 14 03          	movzbl (%ebx,%eax,1),%edx
 444:	88 14 01             	mov    %dl,(%ecx,%eax,1)
 447:	83 c0 01             	add    $0x1,%eax
 44a:	84 d2                	test   %dl,%dl
 44c:	75 f2                	jne    440 <strcpy+0x10>
    ;
  return os;
}
 44e:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 451:	89 c8                	mov    %ecx,%eax
 453:	c9                   	leave  
 454:	c3                   	ret    
 455:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 45c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000460 <strcmp>:

int
strcmp(const char *p, const char *q)
{
 460:	55                   	push   %ebp
 461:	89 e5                	mov    %esp,%ebp
 463:	53                   	push   %ebx
 464:	8b 55 08             	mov    0x8(%ebp),%edx
 467:	8b 4d 0c             	mov    0xc(%ebp),%ecx
  while(*p && *p == *q)
 46a:	0f b6 02             	movzbl (%edx),%eax
 46d:	84 c0                	test   %al,%al
 46f:	75 17                	jne    488 <strcmp+0x28>
 471:	eb 3a                	jmp    4ad <strcmp+0x4d>
 473:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 477:	90                   	nop
 478:	0f b6 42 01          	movzbl 0x1(%edx),%eax
    p++, q++;
 47c:	83 c2 01             	add    $0x1,%edx
 47f:	8d 59 01             	lea    0x1(%ecx),%ebx
  while(*p && *p == *q)
 482:	84 c0                	test   %al,%al
 484:	74 1a                	je     4a0 <strcmp+0x40>
    p++, q++;
 486:	89 d9                	mov    %ebx,%ecx
  while(*p && *p == *q)
 488:	0f b6 19             	movzbl (%ecx),%ebx
 48b:	38 c3                	cmp    %al,%bl
 48d:	74 e9                	je     478 <strcmp+0x18>
  return (uchar)*p - (uchar)*q;
 48f:	29 d8                	sub    %ebx,%eax
}
 491:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 494:	c9                   	leave  
 495:	c3                   	ret    
 496:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 49d:	8d 76 00             	lea    0x0(%esi),%esi
  return (uchar)*p - (uchar)*q;
 4a0:	0f b6 59 01          	movzbl 0x1(%ecx),%ebx
 4a4:	31 c0                	xor    %eax,%eax
 4a6:	29 d8                	sub    %ebx,%eax
}
 4a8:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 4ab:	c9                   	leave  
 4ac:	c3                   	ret    
  return (uchar)*p - (uchar)*q;
 4ad:	0f b6 19             	movzbl (%ecx),%ebx
 4b0:	31 c0                	xor    %eax,%eax
 4b2:	eb db                	jmp    48f <strcmp+0x2f>
 4b4:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 4bb:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 4bf:	90                   	nop

000004c0 <strlen>:

uint
strlen(const char *s)
{
 4c0:	55                   	push   %ebp
 4c1:	89 e5                	mov    %esp,%ebp
 4c3:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  for(n = 0; s[n]; n++)
 4c6:	80 3a 00             	cmpb   $0x0,(%edx)
 4c9:	74 15                	je     4e0 <strlen+0x20>
 4cb:	31 c0                	xor    %eax,%eax
 4cd:	8d 76 00             	lea    0x0(%esi),%esi
 4d0:	83 c0 01             	add    $0x1,%eax
 4d3:	80 3c 02 00          	cmpb   $0x0,(%edx,%eax,1)
 4d7:	89 c1                	mov    %eax,%ecx
 4d9:	75 f5                	jne    4d0 <strlen+0x10>
    ;
  return n;
}
 4db:	89 c8                	mov    %ecx,%eax
 4dd:	5d                   	pop    %ebp
 4de:	c3                   	ret    
 4df:	90                   	nop
  for(n = 0; s[n]; n++)
 4e0:	31 c9                	xor    %ecx,%ecx
}
 4e2:	5d                   	pop    %ebp
 4e3:	89 c8                	mov    %ecx,%eax
 4e5:	c3                   	ret    
 4e6:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 4ed:	8d 76 00             	lea    0x0(%esi),%esi

000004f0 <memset>:

void*
memset(void *dst, int c, uint n)
{
 4f0:	55                   	push   %ebp
 4f1:	89 e5                	mov    %esp,%ebp
 4f3:	57                   	push   %edi
 4f4:	8b 55 08             	mov    0x8(%ebp),%edx
}

static inline void
stosb(void *addr, int data, int cnt)
{
  asm volatile("cld; rep stosb" :
 4f7:	8b 4d 10             	mov    0x10(%ebp),%ecx
 4fa:	8b 45 0c             	mov    0xc(%ebp),%eax
 4fd:	89 d7                	mov    %edx,%edi
 4ff:	fc                   	cld    
 500:	f3 aa                	rep stos %al,%es:(%edi)
  stosb(dst, c, n);
  return dst;
}
 502:	8b 7d fc             	mov    -0x4(%ebp),%edi
 505:	89 d0                	mov    %edx,%eax
 507:	c9                   	leave  
 508:	c3                   	ret    
 509:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

00000510 <strchr>:

char*
strchr(const char *s, char c)
{
 510:	55                   	push   %ebp
 511:	89 e5                	mov    %esp,%ebp
 513:	8b 45 08             	mov    0x8(%ebp),%eax
 516:	0f b6 4d 0c          	movzbl 0xc(%ebp),%ecx
  for(; *s; s++)
 51a:	0f b6 10             	movzbl (%eax),%edx
 51d:	84 d2                	test   %dl,%dl
 51f:	75 12                	jne    533 <strchr+0x23>
 521:	eb 1d                	jmp    540 <strchr+0x30>
 523:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 527:	90                   	nop
 528:	0f b6 50 01          	movzbl 0x1(%eax),%edx
 52c:	83 c0 01             	add    $0x1,%eax
 52f:	84 d2                	test   %dl,%dl
 531:	74 0d                	je     540 <strchr+0x30>
    if(*s == c)
 533:	38 d1                	cmp    %dl,%cl
 535:	75 f1                	jne    528 <strchr+0x18>
      return (char*)s;
  return 0;
}
 537:	5d                   	pop    %ebp
 538:	c3                   	ret    
 539:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
  return 0;
 540:	31 c0                	xor    %eax,%eax
}
 542:	5d                   	pop    %ebp
 543:	c3                   	ret    
 544:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 54b:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 54f:	90                   	nop

00000550 <gets>:

char*
gets(char *buf, int max)
{
 550:	55                   	push   %ebp
 551:	89 e5                	mov    %esp,%ebp
 553:	57                   	push   %edi
 554:	56                   	push   %esi
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
 555:	8d 7d e7             	lea    -0x19(%ebp),%edi
{
 558:	53                   	push   %ebx
  for(i=0; i+1 < max; ){
 559:	31 db                	xor    %ebx,%ebx
{
 55b:	83 ec 1c             	sub    $0x1c,%esp
  for(i=0; i+1 < max; ){
 55e:	eb 27                	jmp    587 <gets+0x37>
    cc = read(0, &c, 1);
 560:	83 ec 04             	sub    $0x4,%esp
 563:	6a 01                	push   $0x1
 565:	57                   	push   %edi
 566:	6a 00                	push   $0x0
 568:	e8 2e 01 00 00       	call   69b <read>
    if(cc < 1)
 56d:	83 c4 10             	add    $0x10,%esp
 570:	85 c0                	test   %eax,%eax
 572:	7e 1d                	jle    591 <gets+0x41>
      break;
    buf[i++] = c;
 574:	0f b6 45 e7          	movzbl -0x19(%ebp),%eax
 578:	8b 55 08             	mov    0x8(%ebp),%edx
 57b:	88 44 1a ff          	mov    %al,-0x1(%edx,%ebx,1)
    if(c == '\n' || c == '\r')
 57f:	3c 0a                	cmp    $0xa,%al
 581:	74 1d                	je     5a0 <gets+0x50>
 583:	3c 0d                	cmp    $0xd,%al
 585:	74 19                	je     5a0 <gets+0x50>
  for(i=0; i+1 < max; ){
 587:	89 de                	mov    %ebx,%esi
 589:	83 c3 01             	add    $0x1,%ebx
 58c:	3b 5d 0c             	cmp    0xc(%ebp),%ebx
 58f:	7c cf                	jl     560 <gets+0x10>
      break;
  }
  buf[i] = '\0';
 591:	8b 45 08             	mov    0x8(%ebp),%eax
 594:	c6 04 30 00          	movb   $0x0,(%eax,%esi,1)
  return buf;
}
 598:	8d 65 f4             	lea    -0xc(%ebp),%esp
 59b:	5b                   	pop    %ebx
 59c:	5e                   	pop    %esi
 59d:	5f                   	pop    %edi
 59e:	5d                   	pop    %ebp
 59f:	c3                   	ret    
  buf[i] = '\0';
 5a0:	8b 45 08             	mov    0x8(%ebp),%eax
 5a3:	89 de                	mov    %ebx,%esi
 5a5:	c6 04 30 00          	movb   $0x0,(%eax,%esi,1)
}
 5a9:	8d 65 f4             	lea    -0xc(%ebp),%esp
 5ac:	5b                   	pop    %ebx
 5ad:	5e                   	pop    %esi
 5ae:	5f                   	pop    %edi
 5af:	5d                   	pop    %ebp
 5b0:	c3                   	ret    
 5b1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 5b8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 5bf:	90                   	nop

000005c0 <stat>:

int
stat(const char *n, struct stat *st)
{
 5c0:	55                   	push   %ebp
 5c1:	89 e5                	mov    %esp,%ebp
 5c3:	56                   	push   %esi
 5c4:	53                   	push   %ebx
  int fd;
  int r;

  fd = open(n, O_RDONLY);
 5c5:	83 ec 08             	sub    $0x8,%esp
 5c8:	6a 00                	push   $0x0
 5ca:	ff 75 08             	push   0x8(%ebp)
 5cd:	e8 f1 00 00 00       	call   6c3 <open>
  if(fd < 0)
 5d2:	83 c4 10             	add    $0x10,%esp
 5d5:	85 c0                	test   %eax,%eax
 5d7:	78 27                	js     600 <stat+0x40>
    return -1;
  r = fstat(fd, st);
 5d9:	83 ec 08             	sub    $0x8,%esp
 5dc:	ff 75 0c             	push   0xc(%ebp)
 5df:	89 c3                	mov    %eax,%ebx
 5e1:	50                   	push   %eax
 5e2:	e8 f4 00 00 00       	call   6db <fstat>
  close(fd);
 5e7:	89 1c 24             	mov    %ebx,(%esp)
  r = fstat(fd, st);
 5ea:	89 c6                	mov    %eax,%esi
  close(fd);
 5ec:	e8 ba 00 00 00       	call   6ab <close>
  return r;
 5f1:	83 c4 10             	add    $0x10,%esp
}
 5f4:	8d 65 f8             	lea    -0x8(%ebp),%esp
 5f7:	89 f0                	mov    %esi,%eax
 5f9:	5b                   	pop    %ebx
 5fa:	5e                   	pop    %esi
 5fb:	5d                   	pop    %ebp
 5fc:	c3                   	ret    
 5fd:	8d 76 00             	lea    0x0(%esi),%esi
    return -1;
 600:	be ff ff ff ff       	mov    $0xffffffff,%esi
 605:	eb ed                	jmp    5f4 <stat+0x34>
 607:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 60e:	66 90                	xchg   %ax,%ax

00000610 <atoi>:

int
atoi(const char *s)
{
 610:	55                   	push   %ebp
 611:	89 e5                	mov    %esp,%ebp
 613:	53                   	push   %ebx
 614:	8b 55 08             	mov    0x8(%ebp),%edx
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
 617:	0f be 02             	movsbl (%edx),%eax
 61a:	8d 48 d0             	lea    -0x30(%eax),%ecx
 61d:	80 f9 09             	cmp    $0x9,%cl
  n = 0;
 620:	b9 00 00 00 00       	mov    $0x0,%ecx
  while('0' <= *s && *s <= '9')
 625:	77 1e                	ja     645 <atoi+0x35>
 627:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 62e:	66 90                	xchg   %ax,%ax
    n = n*10 + *s++ - '0';
 630:	83 c2 01             	add    $0x1,%edx
 633:	8d 0c 89             	lea    (%ecx,%ecx,4),%ecx
 636:	8d 4c 48 d0          	lea    -0x30(%eax,%ecx,2),%ecx
  while('0' <= *s && *s <= '9')
 63a:	0f be 02             	movsbl (%edx),%eax
 63d:	8d 58 d0             	lea    -0x30(%eax),%ebx
 640:	80 fb 09             	cmp    $0x9,%bl
 643:	76 eb                	jbe    630 <atoi+0x20>
  return n;
}
 645:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 648:	89 c8                	mov    %ecx,%eax
 64a:	c9                   	leave  
 64b:	c3                   	ret    
 64c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

00000650 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 650:	55                   	push   %ebp
 651:	89 e5                	mov    %esp,%ebp
 653:	57                   	push   %edi
 654:	8b 45 10             	mov    0x10(%ebp),%eax
 657:	8b 55 08             	mov    0x8(%ebp),%edx
 65a:	56                   	push   %esi
 65b:	8b 75 0c             	mov    0xc(%ebp),%esi
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
 65e:	85 c0                	test   %eax,%eax
 660:	7e 13                	jle    675 <memmove+0x25>
 662:	01 d0                	add    %edx,%eax
  dst = vdst;
 664:	89 d7                	mov    %edx,%edi
 666:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 66d:	8d 76 00             	lea    0x0(%esi),%esi
    *dst++ = *src++;
 670:	a4                   	movsb  %ds:(%esi),%es:(%edi)
  while(n-- > 0)
 671:	39 f8                	cmp    %edi,%eax
 673:	75 fb                	jne    670 <memmove+0x20>
  return vdst;
}
 675:	5e                   	pop    %esi
 676:	89 d0                	mov    %edx,%eax
 678:	5f                   	pop    %edi
 679:	5d                   	pop    %ebp
 67a:	c3                   	ret    

0000067b <fork>:
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
 67b:	b8 01 00 00 00       	mov    $0x1,%eax
 680:	cd 40                	int    $0x40
 682:	c3                   	ret    

00000683 <exit>:
SYSCALL(exit)
 683:	b8 02 00 00 00       	mov    $0x2,%eax
 688:	cd 40                	int    $0x40
 68a:	c3                   	ret    

0000068b <wait>:
SYSCALL(wait)
 68b:	b8 03 00 00 00       	mov    $0x3,%eax
 690:	cd 40                	int    $0x40
 692:	c3                   	ret    

00000693 <pipe>:
SYSCALL(pipe)
 693:	b8 04 00 00 00       	mov    $0x4,%eax
 698:	cd 40                	int    $0x40
 69a:	c3                   	ret    

0000069b <read>:
SYSCALL(read)
 69b:	b8 05 00 00 00       	mov    $0x5,%eax
 6a0:	cd 40                	int    $0x40
 6a2:	c3                   	ret    

000006a3 <write>:
SYSCALL(write)
 6a3:	b8 10 00 00 00       	mov    $0x10,%eax
 6a8:	cd 40                	int    $0x40
 6aa:	c3                   	ret    

000006ab <close>:
SYSCALL(close)
 6ab:	b8 15 00 00 00       	mov    $0x15,%eax
 6b0:	cd 40                	int    $0x40
 6b2:	c3                   	ret    

000006b3 <kill>:
SYSCALL(kill)
 6b3:	b8 06 00 00 00       	mov    $0x6,%eax
 6b8:	cd 40                	int    $0x40
 6ba:	c3                   	ret    

000006bb <exec>:
SYSCALL(exec)
 6bb:	b8 07 00 00 00       	mov    $0x7,%eax
 6c0:	cd 40                	int    $0x40
 6c2:	c3                   	ret    

000006c3 <open>:
SYSCALL(open)
 6c3:	b8 0f 00 00 00       	mov    $0xf,%eax
 6c8:	cd 40                	int    $0x40
 6ca:	c3                   	ret    

000006cb <mknod>:
SYSCALL(mknod)
 6cb:	b8 11 00 00 00       	mov    $0x11,%eax
 6d0:	cd 40                	int    $0x40
 6d2:	c3                   	ret    

000006d3 <unlink>:
SYSCALL(unlink)
 6d3:	b8 12 00 00 00       	mov    $0x12,%eax
 6d8:	cd 40                	int    $0x40
 6da:	c3                   	ret    

000006db <fstat>:
SYSCALL(fstat)
 6db:	b8 08 00 00 00       	mov    $0x8,%eax
 6e0:	cd 40                	int    $0x40
 6e2:	c3                   	ret    

000006e3 <link>:
SYSCALL(link)
 6e3:	b8 13 00 00 00       	mov    $0x13,%eax
 6e8:	cd 40                	int    $0x40
 6ea:	c3                   	ret    

000006eb <mkdir>:
SYSCALL(mkdir)
 6eb:	b8 14 00 00 00       	mov    $0x14,%eax
 6f0:	cd 40                	int    $0x40
 6f2:	c3                   	ret    

000006f3 <chdir>:
SYSCALL(chdir)
 6f3:	b8 09 00 00 00       	mov    $0x9,%eax
 6f8:	cd 40                	int    $0x40
 6fa:	c3                   	ret    

000006fb <dup>:
SYSCALL(dup)
 6fb:	b8 0a 00 00 00       	mov    $0xa,%eax
 700:	cd 40                	int    $0x40
 702:	c3                   	ret    

00000703 <getpid>:
SYSCALL(getpid)
 703:	b8 0b 00 00 00       	mov    $0xb,%eax
 708:	cd 40                	int    $0x40
 70a:	c3                   	ret    

0000070b <sbrk>:
SYSCALL(sbrk)
 70b:	b8 0c 00 00 00       	mov    $0xc,%eax
 710:	cd 40                	int    $0x40
 712:	c3                   	ret    

00000713 <sleep>:
SYSCALL(sleep)
 713:	b8 0d 00 00 00       	mov    $0xd,%eax
 718:	cd 40                	int    $0x40
 71a:	c3                   	ret    

0000071b <uptime>:
SYSCALL(uptime)
 71b:	b8 0e 00 00 00       	mov    $0xe,%eax
 720:	cd 40                	int    $0x40
 722:	c3                   	ret    

00000723 <getprio>:
SYSCALL(getprio)
 723:	b8 16 00 00 00       	mov    $0x16,%eax
 728:	cd 40                	int    $0x40
 72a:	c3                   	ret    

0000072b <setprio>:
SYSCALL(setprio)
 72b:	b8 17 00 00 00       	mov    $0x17,%eax
 730:	cd 40                	int    $0x40
 732:	c3                   	ret    
 733:	66 90                	xchg   %ax,%ax
 735:	66 90                	xchg   %ax,%ax
 737:	66 90                	xchg   %ax,%ax
 739:	66 90                	xchg   %ax,%ax
 73b:	66 90                	xchg   %ax,%ax
 73d:	66 90                	xchg   %ax,%ax
 73f:	90                   	nop

00000740 <printint>:
  write(fd, &c, 1);
}

static void
printint(int fd, int xx, int base, int sgn)
{
 740:	55                   	push   %ebp
 741:	89 e5                	mov    %esp,%ebp
 743:	57                   	push   %edi
 744:	56                   	push   %esi
 745:	53                   	push   %ebx
 746:	83 ec 3c             	sub    $0x3c,%esp
 749:	89 4d c4             	mov    %ecx,-0x3c(%ebp)
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
 74c:	89 d1                	mov    %edx,%ecx
{
 74e:	89 45 b8             	mov    %eax,-0x48(%ebp)
  if(sgn && xx < 0){
 751:	85 d2                	test   %edx,%edx
 753:	0f 89 7f 00 00 00    	jns    7d8 <printint+0x98>
 759:	f6 45 08 01          	testb  $0x1,0x8(%ebp)
 75d:	74 79                	je     7d8 <printint+0x98>
    neg = 1;
 75f:	c7 45 bc 01 00 00 00 	movl   $0x1,-0x44(%ebp)
    x = -xx;
 766:	f7 d9                	neg    %ecx
  } else {
    x = xx;
  }

  i = 0;
 768:	31 db                	xor    %ebx,%ebx
 76a:	8d 75 d7             	lea    -0x29(%ebp),%esi
 76d:	8d 76 00             	lea    0x0(%esi),%esi
  do{
    buf[i++] = digits[x % base];
 770:	89 c8                	mov    %ecx,%eax
 772:	31 d2                	xor    %edx,%edx
 774:	89 cf                	mov    %ecx,%edi
 776:	f7 75 c4             	divl   -0x3c(%ebp)
 779:	0f b6 92 c0 0b 00 00 	movzbl 0xbc0(%edx),%edx
 780:	89 45 c0             	mov    %eax,-0x40(%ebp)
 783:	89 d8                	mov    %ebx,%eax
 785:	8d 5b 01             	lea    0x1(%ebx),%ebx
  }while((x /= base) != 0);
 788:	8b 4d c0             	mov    -0x40(%ebp),%ecx
    buf[i++] = digits[x % base];
 78b:	88 14 1e             	mov    %dl,(%esi,%ebx,1)
  }while((x /= base) != 0);
 78e:	39 7d c4             	cmp    %edi,-0x3c(%ebp)
 791:	76 dd                	jbe    770 <printint+0x30>
  if(neg)
 793:	8b 4d bc             	mov    -0x44(%ebp),%ecx
 796:	85 c9                	test   %ecx,%ecx
 798:	74 0c                	je     7a6 <printint+0x66>
    buf[i++] = '-';
 79a:	c6 44 1d d8 2d       	movb   $0x2d,-0x28(%ebp,%ebx,1)
    buf[i++] = digits[x % base];
 79f:	89 d8                	mov    %ebx,%eax
    buf[i++] = '-';
 7a1:	ba 2d 00 00 00       	mov    $0x2d,%edx

  while(--i >= 0)
 7a6:	8b 7d b8             	mov    -0x48(%ebp),%edi
 7a9:	8d 5c 05 d7          	lea    -0x29(%ebp,%eax,1),%ebx
 7ad:	eb 07                	jmp    7b6 <printint+0x76>
 7af:	90                   	nop
    putc(fd, buf[i]);
 7b0:	0f b6 13             	movzbl (%ebx),%edx
 7b3:	83 eb 01             	sub    $0x1,%ebx
  write(fd, &c, 1);
 7b6:	83 ec 04             	sub    $0x4,%esp
 7b9:	88 55 d7             	mov    %dl,-0x29(%ebp)
 7bc:	6a 01                	push   $0x1
 7be:	56                   	push   %esi
 7bf:	57                   	push   %edi
 7c0:	e8 de fe ff ff       	call   6a3 <write>
  while(--i >= 0)
 7c5:	83 c4 10             	add    $0x10,%esp
 7c8:	39 de                	cmp    %ebx,%esi
 7ca:	75 e4                	jne    7b0 <printint+0x70>
}
 7cc:	8d 65 f4             	lea    -0xc(%ebp),%esp
 7cf:	5b                   	pop    %ebx
 7d0:	5e                   	pop    %esi
 7d1:	5f                   	pop    %edi
 7d2:	5d                   	pop    %ebp
 7d3:	c3                   	ret    
 7d4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
  neg = 0;
 7d8:	c7 45 bc 00 00 00 00 	movl   $0x0,-0x44(%ebp)
 7df:	eb 87                	jmp    768 <printint+0x28>
 7e1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 7e8:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 7ef:	90                   	nop

000007f0 <printf>:

// Print to the given fd. Only understands %d, %x, %p, %s.
void
printf(int fd, const char *fmt, ...)
{
 7f0:	55                   	push   %ebp
 7f1:	89 e5                	mov    %esp,%ebp
 7f3:	57                   	push   %edi
 7f4:	56                   	push   %esi
 7f5:	53                   	push   %ebx
 7f6:	83 ec 2c             	sub    $0x2c,%esp
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
 7f9:	8b 5d 0c             	mov    0xc(%ebp),%ebx
{
 7fc:	8b 75 08             	mov    0x8(%ebp),%esi
  for(i = 0; fmt[i]; i++){
 7ff:	0f b6 13             	movzbl (%ebx),%edx
 802:	84 d2                	test   %dl,%dl
 804:	74 6a                	je     870 <printf+0x80>
  ap = (uint*)(void*)&fmt + 1;
 806:	8d 45 10             	lea    0x10(%ebp),%eax
 809:	83 c3 01             	add    $0x1,%ebx
  write(fd, &c, 1);
 80c:	8d 7d e7             	lea    -0x19(%ebp),%edi
  state = 0;
 80f:	31 c9                	xor    %ecx,%ecx
  ap = (uint*)(void*)&fmt + 1;
 811:	89 45 d0             	mov    %eax,-0x30(%ebp)
 814:	eb 36                	jmp    84c <printf+0x5c>
 816:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 81d:	8d 76 00             	lea    0x0(%esi),%esi
 820:	89 4d d4             	mov    %ecx,-0x2c(%ebp)
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
 823:	b9 25 00 00 00       	mov    $0x25,%ecx
      if(c == '%'){
 828:	83 f8 25             	cmp    $0x25,%eax
 82b:	74 15                	je     842 <printf+0x52>
  write(fd, &c, 1);
 82d:	83 ec 04             	sub    $0x4,%esp
 830:	88 55 e7             	mov    %dl,-0x19(%ebp)
 833:	6a 01                	push   $0x1
 835:	57                   	push   %edi
 836:	56                   	push   %esi
 837:	e8 67 fe ff ff       	call   6a3 <write>
 83c:	8b 4d d4             	mov    -0x2c(%ebp),%ecx
      } else {
        putc(fd, c);
 83f:	83 c4 10             	add    $0x10,%esp
  for(i = 0; fmt[i]; i++){
 842:	0f b6 13             	movzbl (%ebx),%edx
 845:	83 c3 01             	add    $0x1,%ebx
 848:	84 d2                	test   %dl,%dl
 84a:	74 24                	je     870 <printf+0x80>
    c = fmt[i] & 0xff;
 84c:	0f b6 c2             	movzbl %dl,%eax
    if(state == 0){
 84f:	85 c9                	test   %ecx,%ecx
 851:	74 cd                	je     820 <printf+0x30>
      }
    } else if(state == '%'){
 853:	83 f9 25             	cmp    $0x25,%ecx
 856:	75 ea                	jne    842 <printf+0x52>
      if(c == 'd'){
 858:	83 f8 25             	cmp    $0x25,%eax
 85b:	0f 84 07 01 00 00    	je     968 <printf+0x178>
 861:	83 e8 63             	sub    $0x63,%eax
 864:	83 f8 15             	cmp    $0x15,%eax
 867:	77 17                	ja     880 <printf+0x90>
 869:	ff 24 85 68 0b 00 00 	jmp    *0xb68(,%eax,4)
        putc(fd, c);
      }
      state = 0;
    }
  }
}
 870:	8d 65 f4             	lea    -0xc(%ebp),%esp
 873:	5b                   	pop    %ebx
 874:	5e                   	pop    %esi
 875:	5f                   	pop    %edi
 876:	5d                   	pop    %ebp
 877:	c3                   	ret    
 878:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 87f:	90                   	nop
  write(fd, &c, 1);
 880:	83 ec 04             	sub    $0x4,%esp
 883:	88 55 d4             	mov    %dl,-0x2c(%ebp)
 886:	6a 01                	push   $0x1
 888:	57                   	push   %edi
 889:	56                   	push   %esi
 88a:	c6 45 e7 25          	movb   $0x25,-0x19(%ebp)
 88e:	e8 10 fe ff ff       	call   6a3 <write>
        putc(fd, c);
 893:	0f b6 55 d4          	movzbl -0x2c(%ebp),%edx
  write(fd, &c, 1);
 897:	83 c4 0c             	add    $0xc,%esp
 89a:	88 55 e7             	mov    %dl,-0x19(%ebp)
 89d:	6a 01                	push   $0x1
 89f:	57                   	push   %edi
 8a0:	56                   	push   %esi
 8a1:	e8 fd fd ff ff       	call   6a3 <write>
        putc(fd, c);
 8a6:	83 c4 10             	add    $0x10,%esp
      state = 0;
 8a9:	31 c9                	xor    %ecx,%ecx
 8ab:	eb 95                	jmp    842 <printf+0x52>
 8ad:	8d 76 00             	lea    0x0(%esi),%esi
        printint(fd, *ap, 16, 0);
 8b0:	83 ec 0c             	sub    $0xc,%esp
 8b3:	b9 10 00 00 00       	mov    $0x10,%ecx
 8b8:	6a 00                	push   $0x0
 8ba:	8b 45 d0             	mov    -0x30(%ebp),%eax
 8bd:	8b 10                	mov    (%eax),%edx
 8bf:	89 f0                	mov    %esi,%eax
 8c1:	e8 7a fe ff ff       	call   740 <printint>
        ap++;
 8c6:	83 45 d0 04          	addl   $0x4,-0x30(%ebp)
 8ca:	83 c4 10             	add    $0x10,%esp
      state = 0;
 8cd:	31 c9                	xor    %ecx,%ecx
 8cf:	e9 6e ff ff ff       	jmp    842 <printf+0x52>
 8d4:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
        s = (char*)*ap;
 8d8:	8b 45 d0             	mov    -0x30(%ebp),%eax
 8db:	8b 10                	mov    (%eax),%edx
        ap++;
 8dd:	83 c0 04             	add    $0x4,%eax
 8e0:	89 45 d0             	mov    %eax,-0x30(%ebp)
        if(s == 0)
 8e3:	85 d2                	test   %edx,%edx
 8e5:	0f 84 8d 00 00 00    	je     978 <printf+0x188>
        while(*s != 0){
 8eb:	0f b6 02             	movzbl (%edx),%eax
      state = 0;
 8ee:	31 c9                	xor    %ecx,%ecx
        while(*s != 0){
 8f0:	84 c0                	test   %al,%al
 8f2:	0f 84 4a ff ff ff    	je     842 <printf+0x52>
 8f8:	89 5d d4             	mov    %ebx,-0x2c(%ebp)
 8fb:	89 d3                	mov    %edx,%ebx
 8fd:	8d 76 00             	lea    0x0(%esi),%esi
  write(fd, &c, 1);
 900:	83 ec 04             	sub    $0x4,%esp
          s++;
 903:	83 c3 01             	add    $0x1,%ebx
 906:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 909:	6a 01                	push   $0x1
 90b:	57                   	push   %edi
 90c:	56                   	push   %esi
 90d:	e8 91 fd ff ff       	call   6a3 <write>
        while(*s != 0){
 912:	0f b6 03             	movzbl (%ebx),%eax
 915:	83 c4 10             	add    $0x10,%esp
 918:	84 c0                	test   %al,%al
 91a:	75 e4                	jne    900 <printf+0x110>
      state = 0;
 91c:	8b 5d d4             	mov    -0x2c(%ebp),%ebx
 91f:	31 c9                	xor    %ecx,%ecx
 921:	e9 1c ff ff ff       	jmp    842 <printf+0x52>
 926:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 92d:	8d 76 00             	lea    0x0(%esi),%esi
        printint(fd, *ap, 10, 1);
 930:	83 ec 0c             	sub    $0xc,%esp
 933:	b9 0a 00 00 00       	mov    $0xa,%ecx
 938:	6a 01                	push   $0x1
 93a:	e9 7b ff ff ff       	jmp    8ba <printf+0xca>
 93f:	90                   	nop
        putc(fd, *ap);
 940:	8b 45 d0             	mov    -0x30(%ebp),%eax
  write(fd, &c, 1);
 943:	83 ec 04             	sub    $0x4,%esp
        putc(fd, *ap);
 946:	8b 00                	mov    (%eax),%eax
  write(fd, &c, 1);
 948:	6a 01                	push   $0x1
 94a:	57                   	push   %edi
 94b:	56                   	push   %esi
        putc(fd, *ap);
 94c:	88 45 e7             	mov    %al,-0x19(%ebp)
  write(fd, &c, 1);
 94f:	e8 4f fd ff ff       	call   6a3 <write>
        ap++;
 954:	83 45 d0 04          	addl   $0x4,-0x30(%ebp)
 958:	83 c4 10             	add    $0x10,%esp
      state = 0;
 95b:	31 c9                	xor    %ecx,%ecx
 95d:	e9 e0 fe ff ff       	jmp    842 <printf+0x52>
 962:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        putc(fd, c);
 968:	88 55 e7             	mov    %dl,-0x19(%ebp)
  write(fd, &c, 1);
 96b:	83 ec 04             	sub    $0x4,%esp
 96e:	e9 2a ff ff ff       	jmp    89d <printf+0xad>
 973:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 977:	90                   	nop
          s = "(null)";
 978:	ba 61 0b 00 00       	mov    $0xb61,%edx
        while(*s != 0){
 97d:	89 5d d4             	mov    %ebx,-0x2c(%ebp)
 980:	b8 28 00 00 00       	mov    $0x28,%eax
 985:	89 d3                	mov    %edx,%ebx
 987:	e9 74 ff ff ff       	jmp    900 <printf+0x110>
 98c:	66 90                	xchg   %ax,%ax
 98e:	66 90                	xchg   %ax,%ax

00000990 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 990:	55                   	push   %ebp
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 991:	a1 9c 0e 00 00       	mov    0xe9c,%eax
{
 996:	89 e5                	mov    %esp,%ebp
 998:	57                   	push   %edi
 999:	56                   	push   %esi
 99a:	53                   	push   %ebx
 99b:	8b 5d 08             	mov    0x8(%ebp),%ebx
  bp = (Header*)ap - 1;
 99e:	8d 4b f8             	lea    -0x8(%ebx),%ecx
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 9a1:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 9a8:	89 c2                	mov    %eax,%edx
 9aa:	8b 00                	mov    (%eax),%eax
 9ac:	39 ca                	cmp    %ecx,%edx
 9ae:	73 30                	jae    9e0 <free+0x50>
 9b0:	39 c1                	cmp    %eax,%ecx
 9b2:	72 04                	jb     9b8 <free+0x28>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 9b4:	39 c2                	cmp    %eax,%edx
 9b6:	72 f0                	jb     9a8 <free+0x18>
      break;
  if(bp + bp->s.size == p->s.ptr){
 9b8:	8b 73 fc             	mov    -0x4(%ebx),%esi
 9bb:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 9be:	39 f8                	cmp    %edi,%eax
 9c0:	74 30                	je     9f2 <free+0x62>
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
 9c2:	89 43 f8             	mov    %eax,-0x8(%ebx)
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
 9c5:	8b 42 04             	mov    0x4(%edx),%eax
 9c8:	8d 34 c2             	lea    (%edx,%eax,8),%esi
 9cb:	39 f1                	cmp    %esi,%ecx
 9cd:	74 3a                	je     a09 <free+0x79>
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
 9cf:	89 0a                	mov    %ecx,(%edx)
  } else
    p->s.ptr = bp;
  freep = p;
}
 9d1:	5b                   	pop    %ebx
  freep = p;
 9d2:	89 15 9c 0e 00 00    	mov    %edx,0xe9c
}
 9d8:	5e                   	pop    %esi
 9d9:	5f                   	pop    %edi
 9da:	5d                   	pop    %ebp
 9db:	c3                   	ret    
 9dc:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 9e0:	39 c2                	cmp    %eax,%edx
 9e2:	72 c4                	jb     9a8 <free+0x18>
 9e4:	39 c1                	cmp    %eax,%ecx
 9e6:	73 c0                	jae    9a8 <free+0x18>
  if(bp + bp->s.size == p->s.ptr){
 9e8:	8b 73 fc             	mov    -0x4(%ebx),%esi
 9eb:	8d 3c f1             	lea    (%ecx,%esi,8),%edi
 9ee:	39 f8                	cmp    %edi,%eax
 9f0:	75 d0                	jne    9c2 <free+0x32>
    bp->s.size += p->s.ptr->s.size;
 9f2:	03 70 04             	add    0x4(%eax),%esi
 9f5:	89 73 fc             	mov    %esi,-0x4(%ebx)
    bp->s.ptr = p->s.ptr->s.ptr;
 9f8:	8b 02                	mov    (%edx),%eax
 9fa:	8b 00                	mov    (%eax),%eax
 9fc:	89 43 f8             	mov    %eax,-0x8(%ebx)
  if(p + p->s.size == bp){
 9ff:	8b 42 04             	mov    0x4(%edx),%eax
 a02:	8d 34 c2             	lea    (%edx,%eax,8),%esi
 a05:	39 f1                	cmp    %esi,%ecx
 a07:	75 c6                	jne    9cf <free+0x3f>
    p->s.size += bp->s.size;
 a09:	03 43 fc             	add    -0x4(%ebx),%eax
  freep = p;
 a0c:	89 15 9c 0e 00 00    	mov    %edx,0xe9c
    p->s.size += bp->s.size;
 a12:	89 42 04             	mov    %eax,0x4(%edx)
    p->s.ptr = bp->s.ptr;
 a15:	8b 4b f8             	mov    -0x8(%ebx),%ecx
 a18:	89 0a                	mov    %ecx,(%edx)
}
 a1a:	5b                   	pop    %ebx
 a1b:	5e                   	pop    %esi
 a1c:	5f                   	pop    %edi
 a1d:	5d                   	pop    %ebp
 a1e:	c3                   	ret    
 a1f:	90                   	nop

00000a20 <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 a20:	55                   	push   %ebp
 a21:	89 e5                	mov    %esp,%ebp
 a23:	57                   	push   %edi
 a24:	56                   	push   %esi
 a25:	53                   	push   %ebx
 a26:	83 ec 1c             	sub    $0x1c,%esp
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 a29:	8b 45 08             	mov    0x8(%ebp),%eax
  if((prevp = freep) == 0){
 a2c:	8b 3d 9c 0e 00 00    	mov    0xe9c,%edi
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 a32:	8d 70 07             	lea    0x7(%eax),%esi
 a35:	c1 ee 03             	shr    $0x3,%esi
 a38:	83 c6 01             	add    $0x1,%esi
  if((prevp = freep) == 0){
 a3b:	85 ff                	test   %edi,%edi
 a3d:	0f 84 9d 00 00 00    	je     ae0 <malloc+0xc0>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 a43:	8b 17                	mov    (%edi),%edx
    if(p->s.size >= nunits){
 a45:	8b 4a 04             	mov    0x4(%edx),%ecx
 a48:	39 f1                	cmp    %esi,%ecx
 a4a:	73 6a                	jae    ab6 <malloc+0x96>
 a4c:	bb 00 10 00 00       	mov    $0x1000,%ebx
 a51:	39 de                	cmp    %ebx,%esi
 a53:	0f 43 de             	cmovae %esi,%ebx
  p = sbrk(nu * sizeof(Header));
 a56:	8d 04 dd 00 00 00 00 	lea    0x0(,%ebx,8),%eax
 a5d:	89 45 e4             	mov    %eax,-0x1c(%ebp)
 a60:	eb 17                	jmp    a79 <malloc+0x59>
 a62:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 a68:	8b 02                	mov    (%edx),%eax
    if(p->s.size >= nunits){
 a6a:	8b 48 04             	mov    0x4(%eax),%ecx
 a6d:	39 f1                	cmp    %esi,%ecx
 a6f:	73 4f                	jae    ac0 <malloc+0xa0>
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 a71:	8b 3d 9c 0e 00 00    	mov    0xe9c,%edi
 a77:	89 c2                	mov    %eax,%edx
 a79:	39 d7                	cmp    %edx,%edi
 a7b:	75 eb                	jne    a68 <malloc+0x48>
  p = sbrk(nu * sizeof(Header));
 a7d:	83 ec 0c             	sub    $0xc,%esp
 a80:	ff 75 e4             	push   -0x1c(%ebp)
 a83:	e8 83 fc ff ff       	call   70b <sbrk>
  if(p == (char*)-1)
 a88:	83 c4 10             	add    $0x10,%esp
 a8b:	83 f8 ff             	cmp    $0xffffffff,%eax
 a8e:	74 1c                	je     aac <malloc+0x8c>
  hp->s.size = nu;
 a90:	89 58 04             	mov    %ebx,0x4(%eax)
  free((void*)(hp + 1));
 a93:	83 ec 0c             	sub    $0xc,%esp
 a96:	83 c0 08             	add    $0x8,%eax
 a99:	50                   	push   %eax
 a9a:	e8 f1 fe ff ff       	call   990 <free>
  return freep;
 a9f:	8b 15 9c 0e 00 00    	mov    0xe9c,%edx
      if((p = morecore(nunits)) == 0)
 aa5:	83 c4 10             	add    $0x10,%esp
 aa8:	85 d2                	test   %edx,%edx
 aaa:	75 bc                	jne    a68 <malloc+0x48>
        return 0;
  }
}
 aac:	8d 65 f4             	lea    -0xc(%ebp),%esp
        return 0;
 aaf:	31 c0                	xor    %eax,%eax
}
 ab1:	5b                   	pop    %ebx
 ab2:	5e                   	pop    %esi
 ab3:	5f                   	pop    %edi
 ab4:	5d                   	pop    %ebp
 ab5:	c3                   	ret    
    if(p->s.size >= nunits){
 ab6:	89 d0                	mov    %edx,%eax
 ab8:	89 fa                	mov    %edi,%edx
 aba:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
      if(p->s.size == nunits)
 ac0:	39 ce                	cmp    %ecx,%esi
 ac2:	74 4c                	je     b10 <malloc+0xf0>
        p->s.size -= nunits;
 ac4:	29 f1                	sub    %esi,%ecx
 ac6:	89 48 04             	mov    %ecx,0x4(%eax)
        p += p->s.size;
 ac9:	8d 04 c8             	lea    (%eax,%ecx,8),%eax
        p->s.size = nunits;
 acc:	89 70 04             	mov    %esi,0x4(%eax)
      freep = prevp;
 acf:	89 15 9c 0e 00 00    	mov    %edx,0xe9c
}
 ad5:	8d 65 f4             	lea    -0xc(%ebp),%esp
      return (void*)(p + 1);
 ad8:	83 c0 08             	add    $0x8,%eax
}
 adb:	5b                   	pop    %ebx
 adc:	5e                   	pop    %esi
 add:	5f                   	pop    %edi
 ade:	5d                   	pop    %ebp
 adf:	c3                   	ret    
    base.s.ptr = freep = prevp = &base;
 ae0:	c7 05 9c 0e 00 00 a0 	movl   $0xea0,0xe9c
 ae7:	0e 00 00 
    base.s.size = 0;
 aea:	bf a0 0e 00 00       	mov    $0xea0,%edi
    base.s.ptr = freep = prevp = &base;
 aef:	c7 05 a0 0e 00 00 a0 	movl   $0xea0,0xea0
 af6:	0e 00 00 
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 af9:	89 fa                	mov    %edi,%edx
    base.s.size = 0;
 afb:	c7 05 a4 0e 00 00 00 	movl   $0x0,0xea4
 b02:	00 00 00 
    if(p->s.size >= nunits){
 b05:	e9 42 ff ff ff       	jmp    a4c <malloc+0x2c>
 b0a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
        prevp->s.ptr = p->s.ptr;
 b10:	8b 08                	mov    (%eax),%ecx
 b12:	89 0a                	mov    %ecx,(%edx)
 b14:	eb b9                	jmp    acf <malloc+0xaf>
