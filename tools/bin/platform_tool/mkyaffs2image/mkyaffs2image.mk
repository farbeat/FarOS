yaffs2image:
	cd $(MKYAFFS2DIR); make;
	cp $(MKYAFFS2DIR)/mkyaffs2image $(LITEOSTOPDIR)/tools/fsimage

yaffs2imageclean:
	cd $(MKYAFFS2DIR); make clean;
	@-$(RM) $(LITEOSTOPDIR)/tools/fsimage/mkyaffs2image
	@echo "clean yaffs2 tool finish"

.PHONY : yaffs2image yaffs2imageclean
