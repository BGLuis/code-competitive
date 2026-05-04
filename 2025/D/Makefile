.PHONY: all test test-c test-cpp test-py3

all: test

test-c:
	gcc answers/c/solution.c -O3 -lm -o solution_c
	@echo "Testing D in c..."
	@for f in $$(ls input/); do \
		./solution_c < input/$$f > tmp_out; \
		bash compare/c tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in c"; fi; \
	done
	@rm -f tmp_out solution_c

test-cpp:
	g++ answers/cpp/solution.cpp -O3 -o solution_cpp
	@echo "Testing D in cpp..."
	@for f in $$(ls input/); do \
		./solution_cpp < input/$$f > tmp_out; \
		bash compare/cpp tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in cpp"; fi; \
	done
	@rm -f tmp_out solution_cpp

test-py3:
	@echo "Testing D in py3..."
	@for f in $$(ls input/); do \
		python3 answers/py3/solution.py < input/$$f > tmp_out; \
		bash compare/py3 tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in py3"; fi; \
	done
	@rm -f tmp_out

test: test-c test-cpp test-py3
