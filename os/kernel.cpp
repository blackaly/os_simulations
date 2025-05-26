extern "C" void kernel_main()
{
	char* vid_mem = (char*) 0xb8000;
	for(int i = 0; i < 80 * 25; ++i)
	{
		vid_mem[i*2] =  ' ';
		vid_mem[i*2+1] = 0x07;
	}

	const char* m = "Hello World, I am Ali Mashally.. I will achieve my dreams some day :)";
	int i = 0;
	while(m[i] != '\0')
	{
		int pos = i * 2;
		vid_mem[pos] = m[i];
		vid_mem[pos + 1] = 0x0F;
		i++;
	}

	while(1){asm volatile("hlt");}
}
