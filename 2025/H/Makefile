.PHONY: all test test-c test-cpp test-java test-kt test-py3

all: test

test-c:
	gcc answers/c/solution.c -O3 -lm -o solution_c
	@echo "Testing H in c..."
	@for f in $$(ls input/); do \
		./solution_c < input/$$f > tmp_out; \
		python3 compare/c tmp_out output/$$f input/$$f > /dev/null 2> /dev/null || echo "Fail $$f in c"; \
	done
	@rm -f tmp_out solution_c

test-cpp:
	g++ answers/cpp/solution.cpp -O3 -o solution_cpp
	@echo "Testing H in cpp..."
	@for f in $$(ls input/); do \
		./solution_cpp < input/$$f > tmp_out; \
		python3 compare/cpp tmp_out output/$$f input/$$f > /dev/null 2> /dev/null || echo "Fail $$f in cpp"; \
	done
	@rm -f tmp_out solution_cpp

test-java:
	javac answers/java/Solution.java
	@echo "Testing H in java..."
	@for f in $$(ls input/); do \
		java -cp answers/java Solution < input/$$f > tmp_out; \
		python3 compare/java tmp_out output/$$f input/$$f > /dev/null 2> /dev/null || echo "Fail $$f in java"; \
	done
	@rm -f tmp_out answers/java/*.class

test-py3:
	@echo "Testing H in py3..."
	@for f in $$(ls input/); do \
		python3 answers/py3/solution.py < input/$$f > tmp_out; \
		python3 compare/py3 tmp_out output/$$f input/$$f > /dev/null 2> /dev/null || echo "Fail $$f in py3"; \
	done
	@rm -f tmp_out

test: test-c test-cpp test-java test-py3
