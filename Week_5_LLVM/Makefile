LLVM_CONFIG=llvm-config-15
OPT=/usr/bin/opt-15
PASS_NAME=MyPass
SRC=NewPass.cpp
SO=NewPass.so
TESTS=test.ll test2.ll

all: $(SO)

$(SO): $(SRC)
	g++ -fPIC -shared -o $(SO) $(SRC) `$$( $(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core )`

build: $(SO)
	@for t in $(TESTS); do \
		echo "=== Running on $$t ==="; \
		$(OPT) -load-pass-plugin ./$(SO) -passes=$(PASS_NAME) -disable-output $$t; \
	done

clean:
	rm -f $(SO)

