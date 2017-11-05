CC = $(shell which clang gcc | head --lines=1)
CXX = $(shell which clang++ g++ | head --lines=1)
CFLAGS = -W -Wall -Wextra -pedantic -O3 -flto -DNDEBUG

ez80sf.rom: external/fasmg ez80sf.src
	@git submodule update --init --recursive -- external/fasmg-ez80
	$^ $@
ez80sf.src: src/*.inc
	@for file in external/fasmg-ez80/{ez80,symbol_table}.inc $^; do
		echo "include '$$file'"
	done >$@

external/fasmg: external/fasmg.zip
	unzip -o $< -d $(@D) $(@F)
	chmod +x $@
external/fasmg.zip:
	mkdir -p $(@D)
	cd $(@D)
	wget https://flatassembler.net/fasmg.zip

check: ez80sf.rom
	@git submodule update --init --recursive -- external/CEmu
	$(MAKE) -C external/CEmu/core CC="$(CC)" CXX="$(CXX)" CFLAGS="$(CFLAGS)" libcemucore.a
	grep "^\w\+: *; *CHECK: *" src/*.src | while read -a args; do
		$(CC) $(CFLAGS) -I external/CEmu/core test/tester.c external/CEmu/core/libcemucore.a -o test/tester
		echo -n "Testing $${args[0]} "
		test/tester $^ `grep "^$${args[0]%%:*} = " ez80sf.lab | cut -d\  -f3`
	done

clean:
	$(MAKE) -C test/CEmu/core clean
	rm --force --recursive ez80sf.* test/tester external/fasmg
distclean:
	git submodule deinit --all
	rm --force --recursive ez80sf.* test/tester external/fasmg

.INTERMEDIATE: ez80sf.src external/fasmg.zip
.PHONY: check clean distclean
.ONESHELL:
