struct sub_input {
	char s[10];
	float *f_sub;
	struct sub_input *self;
};

struct lib_input {
    int *i_ptr;
    double *d_ptr;
    float f;
	struct sub_input sub; /* we need to handle substructs :/ */
	struct sub_input *sub_ptr;
};
