.PHONY: clean All

All:
	@echo "----------Building project:[ thenextweek - Debug ]----------"
	@"$(MAKE)" -f  "thenextweek.mk"
clean:
	@echo "----------Cleaning project:[ thenextweek - Debug ]----------"
	@"$(MAKE)" -f  "thenextweek.mk" clean
