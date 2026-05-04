.PHONY: all test test-cpp test-java test-py3

all: test

test-cpp:
	g++ answers/cpp/solution.cpp -O3 -o solution_cpp
	@echo "Testing E in cpp..."
	@for f in $$(ls input/); do \
		./solution_cpp < input/$$f > tmp_out; \
		bash compare/cpp tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in cpp"; fi; \
	done
	@rm -f tmp_out solution_cpp

test-java:
	javac answers/java/Solution.java
	@echo "Testing E in java..."
	@for f in $$(ls input/); do \
		java -cp answers/java Solution < input/$$f > tmp_out; \
		bash compare/java tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in java"; fi; \
	done
	@rm -f tmp_out answers/java/*.class

test-py3:
	@echo "Testing E in py3..."
	@for f in $$(ls input/); do \
		python3 answers/py3/solution.py < input/$$f > tmp_out; \
		bash compare/py3 tmp_out output/$$f input/$$f > /dev/null 2> /dev/null; \
		if [ \$\$? -ne 4 ]; then echo "Fail \$\$f in py3"; fi; \
	done
	@rm -f tmp_out

test: test-cpp test-java test-py3
