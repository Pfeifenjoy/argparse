#ifndef ARGUMENT_H_TUQ0BVOC
#define ARGUMENT_H_TUQ0BVOC

enum argument_type_t {
	ARGUMENT_STRING,
	ARGUMENT_BOOLEAN,
	ARGUMENT_INT,
	ARGUMENT_FLOAT
};

struct argument {
	const char *name;
	enum argument_type_t type;
};


#endif /* end of include guard: ARGUMENT_H_TUQ0BVOC */
