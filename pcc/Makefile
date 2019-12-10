include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=xt_pcc
PKG_RELEASE:=1

PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/iptables-mod-pcc
  SUBMENU:=Firewall
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Per Connection Classifier Match Module
  DEPENDS:=+iptables +kmod-ipt-pcc
  MAINTAINER:=SZLZNET <szlznet@126.com>
endef

define Package/iptables-mod-pcc/install
	$(INSTALL_DIR) $(1)/usr/lib/iptables
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libipt_pcc.so $(1)/usr/lib/iptables
endef

define KernelPackage/ipt-pcc
  SUBMENU:=Netfilter Extensions
  TITLE:=Per Connection Classifier Match Module
  DEPENDS:=+kmod-ipt-nat
  MAINTAINER:=SZLZNET <szlznet@126.com>
  FILES:=$(PKG_BUILD_DIR)/xt_pcc.ko
endef

include $(INCLUDE_DIR)/kernel-defaults.mk

define Build/Prepare
	$(call Build/Prepare/Default)
	# $(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
        CROSS_COMPILE="$(TARGET_CROSS)" \
        ARCH="$(LINUX_KARCH)" \
        SUBDIRS="$(PKG_BUILD_DIR)" \
        EXTRA_CFLAGS="$(BUILDFLAGS)" \
        modules
	$(call Build/Compile/Default)
endef

$(eval $(call BuildPackage,iptables-mod-pcc))
$(eval $(call KernelPackage,ipt-pcc))
