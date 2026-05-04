.PHONY: all test test-cpp

all: test

test-cpp:
	g++ answers/cpp/solution.cpp -O3 -o solution_cpp
	@echo "Testing G in cpp..."
	@for f in $$(ls input/); do \
		./solution_cpp < input/$$f > tmp_out; \
		python3 compare/cpp tmp_out output/$$f input/$$f > /dev/null 2> /dev/null || echo "Fail $$f in cpp"; \
	done
	@rm -f tmp_out solution_cpp

test: test-cpp
