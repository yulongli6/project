oj_server:oj_server.cpp oj_model.hpp oj_view.hpp
	g++ $< -o $@ -std=c++11 -lpthread -ljsoncpp\
		-lctemplate\
		-I ~/third_part/include -L ~/third_part/lib


compile_serve:compile_serve.cpp compile.hpp
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp
test:test.cpp
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp
.PHONY:clen
clen:
	-rm oj_server
