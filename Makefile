all:
	clang tinymm_test.c tinymm.c -o tinymm_test; ./tinymm_test; echo $$?
