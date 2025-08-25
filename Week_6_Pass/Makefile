LLVM_CONFIG=llvm-config-15
OPT=opt-15
CXX=g++
PASS_NAME=gcp
SRC=NewPass.cpp
SO=NewPass.so
TESTS=test.ll test2.ll test3.ll test4.ll

CXXFLAGS   := -fPIC -shared -std=c++17 `$(LLVM_CONFIG) --cxxflags`
LDFLAGS    := `$(LLVM_CONFIG) --ldflags --system-libs --libs core passes`

all: $(SO)

$(SO): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

#Прогон: сначала работаю с mem2reg, чтобы не писать самому это, затем уже свой основной пасс
build: $(SO)
	@for t in $(TESTS); do \
		echo "=== $$t after mem2reg + $(PASS_NAME) ==="; \
		$(OPT) -load-pass-plugin ./$(SO) -passes="mem2reg,$(PASS_NAME)" -S $$t -o -; \
	done

clean:
	rm -f $(SO)

