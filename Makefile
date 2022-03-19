CC = $(shell which clang gcc | head -n1)
CXX = $(shell which clang++ g++ | head -n1)
CFLAGS = -W -Wall -Wextra -pedantic -O3 -flto -DNDEBUG

CHECK = *
OFFSET = 0
ITERATIONS = 1000000
MAX_CYCLES = 15000
FASMG_VERSION = jje9

rwildcard = $(strip $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2) $(filter $(subst %%,%,%$(subst *,%,$2)),$d)))

ez80sf.rom: external/fasmg ez80sf.src
	@git submodule update --init --recursive -- external/fasmg-ez80
	$^ $@
ez80sf.src: $(sort $(call rwildcard,src,*.inc))
	@{ \
		for file in $(addprefix external/fasmg-ez80/,ez80.inc symbol_table.inc) $^; do \
			printf "include '$$file'\n"; \
		done; \
	} >$@

external/fasmg: external/fasmg.$(FASMG_VERSION).zip
	unzip -j -o $< -d $(@D) `test "\`uname -s\`" = Darwin && printf source/macos/x64/`$(@F)
	chmod +x $@
external/fasmg.$(FASMG_VERSION).zip:
	wget https://flatassembler.net/fasmg.$(FASMG_VERSION).zip --output-document=$@

check: ez80sf.rom
	@git submodule update --init --recursive -- external/CEmu
	$(MAKE) -C external/CEmu/core CC="$(CC)" CXX="$(CXX)" CFLAGS="$(CFLAGS)" libcemucore.a
	@sed -E -e '/^\??([[:alnum:]_.]+) *:(= *[[:alnum:]_.]+)? *;.*CHECK:/!d;s/^\??([[:alnum:]_.]+) *:(= *[[:alnum:]_.]+)? *; *(PREREQ:(.*))?CHECK:(.*)$$/\1\t\4\t\5/;s/\t\t/\ttrue\t/' $(patsubst %,src/%.inc,$(CHECK)) | { \
		exit=0; IFS=\	; while read -r label prereq check; do \
			$(CC) $(CFLAGS) $(LDFLAGS) -I external/CEmu/core -DPREREQ="$$prereq" -DCHECK="$${check#*CHECK:}" -DOFFSET="$(OFFSET)" -DITERATIONS="$(ITERATIONS)" \
				-DMAX_CYCLES="$(MAX_CYCLES)" test/tester.c external/CEmu/core/libcemucore.a -lm -o test/tester && \
			printf '%s' "Testing $$label..." >&2 && \
			test/tester $^ "`grep "^$$label *= *" ez80sf.lab | cut -d= -f2`" || \
			exit=1; \
		done; exit $$exit; \
	}

clean:
	$(MAKE) -C external/CEmu/core clean
	rm -rf ez80sf.* test/tester
distclean:
	git submodule deinit --all --force
	rm -rf ez80sf.* test/tester external/fasmg

.INTERMEDIATE: ez80sf.src external/fasmg.$(FASMG_VERSION).zip
.PHONY: check clean distclean
