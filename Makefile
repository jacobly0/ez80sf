CC = $(shell which clang gcc | head -n1)
CXX = $(shell which clang++ g++ | head -n1)
CFLAGS = -W -Wall -Wextra -pedantic -O3 -flto -DNDEBUG

CHECK = *
OFFSET = 0
ITERATIONS = 1000000
MAX_CYCLES = 15000

ez80sf.rom: external/fasmg ez80sf.src
	@git submodule update --init --recursive -- external/fasmg-ez80
	$^ $@
ez80sf.src: src/*.inc
	@{ \
		for file in $(addprefix external/fasmg-ez80/,ez80.inc symbol_table.inc) $^; do \
			printf "include '$$file'\n"; \
		done; \
	} >$@

external/fasmg: external/fasmg.zip
	unzip -j -o $< -d $(@D) `test "\`uname -s\`" = Darwin && printf source/macos/`$(@F)
	chmod +x $@
external/fasmg.zip:
	wget https://flatassembler.net/fasmg.zip --output-document=$@

check: ez80sf.rom
	@git submodule update --init --recursive -- external/CEmu
	$(MAKE) -C external/CEmu/core CC="$(CC)" CXX="$(CXX)" CFLAGS="$(CFLAGS)" libcemucore.a
	@grep -h "^?\?\w\+: *; *CHECK: *" $(patsubst %,src/%.inc,$(CHECK)) | { \
		exit=0; while read check; do \
			$(CC) $(CFLAGS) $(LDFLAGS) -I external/CEmu/core -DCHECK="$${check##*:}" -DOFFSET="$(OFFSET)" -DITERATIONS="$(ITERATIONS)" \
				-DMAX_CYCLES="$(MAX_CYCLES)" test/tester.c external/CEmu/core/libcemucore.a -lm -o test/tester && \
			printf '%s' "Testing $${check%%:*}..." && \
			test/tester $^ "`grep "^$${check%%:*} *= *" ez80sf.lab | cut -d= -f2`" || \
			exit=1; \
		done; exit $$exit; \
	}

clean:
	$(MAKE) -C external/CEmu/core clean
	rm -rf ez80sf.* test/tester
distclean:
	git submodule deinit --all --force
	rm -rf ez80sf.* test/tester external/fasmg

.INTERMEDIATE: ez80sf.src external/fasmg.zip
.PHONY: check clean distclean
