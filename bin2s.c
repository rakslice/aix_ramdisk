
#include <stdio.h>


#define MIN(a,b) ((a)<(b)?(a):(b))


int main(int argc, char ** argv) {
	FILE * f;
	int sz;
	int ret;
	char * ch;

	if (argc < 2) {
		printf("Usage:\n");
		printf(" %s {input_filename}\n", argv[0]);
		return 1;
	}
		
	f = fopen(argv[1], "rb");

	if (f == NULL) {
		printf("%s: Error opening %s\n", argv[0], argv[1]);
		return 1;
	}


	fseek(f, 0L, SEEK_END);
	sz = ftell(f);
	fseek(f, 0L, SEEK_SET);	

	// basenamify
	for (ch = argv[1]; *ch != '\0'; ch++) {
		if (*ch == '.') {
			*ch = '\0';
			break;
		}
	}

	printf(".data\n");
	printf(".globl %s_len\n", argv[1]);
	printf("%s_len:\n", argv[1]);
	printf(".long %d\n", sz);

	printf(".globl %s\n", argv[1]);
	printf("%s:\n", argv[1]);

	unsigned short buf;

	while (sz > 0) {
		buf=0;
		ret = fread(&buf, MIN(2, sz), 1, f);
		if (!ret) {
			fprintf(stderr, "%s: error reading\n", argv[0]);
			return 1;
		}
		printf(".value 0x%04x\n", (int)buf);
		sz -= 2;
	}

	fclose(f);

}
