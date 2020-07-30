# config

TARGET		= diet_tracker
COMPILER 	= g++
FLAGS 		= -Wall -O2
HEADERS 	= 
INCLUDE		= 
LIBS 		= -lreadline -lncurses -lzip
EXT 		= cpp

################################################################################
DIR_SRC		:= src
DIR_OBJ		:= obj
FILES_SRC	:= $(wildcard $(DIR_SRC)/*.$(EXT))
FILES_OBJ	:= $(addprefix $(DIR_OBJ)/, $(notdir $(FILES_SRC:.$(EXT)=.o)))
FILES_HEAD	:= $(wildcard $(DIR_SRC)/*.h)

.PHONY = compile clean
all : compile

compile : $(FILES_OBJ) $(FILES_HEAD)
	@echo "- compiling target..."
	@echo -n "    "
	$(COMPILER) $(FLAGS) $^ -o $(TARGET) $(LIBS) 
	@echo "> done: $(TARGET) compiled"

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.$(EXT) $(FILES_HEAD)
	@echo -n "- compiling $<... "
	@mkdir -p $(DIR_OBJ)
	@$(COMPILER) $(CFLAGS) -I$(DIR_SRC) $(INCLUDE) -c -o $@ $<
	@echo "done"

print :
	@echo "FILES_SRC:  |$(FILES_SRC)|"
	@echo "FILES_OBJ:  |$(FILES_OBJ)|"
	@echo "FILES_HEAD: |$(FILES_HEAD)|"
	@echo " "
	@echo "HEADERS:    |$(HEADERS)|"
	@echo "INCLUDE:    |$(INCLUDE)|"
	@echo "LIBS:       |$(LIBS)|"

clean:
	@echo -n "> cleaning $(TARGET)... "
	@rm -f $(TARGET)
	@rm -rf $(DIR_OBJ)
	@echo "done"