// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/regulator/consumer.h>
#include <video/mipi_display.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>

struct huawei_nt51021 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;

	struct mutex mutex;

	//struct regulator *vddio;
	//struct regulator *lcd_bias;
	struct regulator *vddio;
	struct regulator *vsp;
	struct regulator *vsn;
	//struct regulator *pp3300;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *disp_power_panel;
	struct gpio_desc *disp_power_backlight;
	struct gpio_desc *disp_en_gpio_vled;

	u32 inversion_mode;
	u32 inversion_state;
};

#define NOVATEK_NT51021_DATA_SEND 0x8F //Force sending by MIPI
#define NOVATEK_NT51021_PAGE1 0x83
#define NOVATEK_NT51021_PAGE2 0x84
#define NOVATEK_NT51021_GOP 0x8c
#define NOVATEK_NT51021_3DUMMY 0xcd
#define NOVATEK_NT51021_CABC 0x90
#define NOVATEK_NT51021_GCH 0xc8
#define NOVATEK_NT51021_RESISTOR 0x97
#define NOVATEK_NT51021_BACKLIGHT 0x9f
#define NOVATEK_NT51021_IC_MIPI_RX 0xa9
#define NOVATEK_NT51021_TESTMODE1 0x85
#define NOVATEK_NT51021_TESTMODE2 0x86
#define NOVATEK_NT51021_TESTMODE3 0x9c

#define UNKNOWN_NT51021_A1 0xa1
#define UNKNOWN_NT51021_A2 0xa2
#define UNKNOWN_NT51021_A3 0xa3
#define UNKNOWN_NT51021_A4 0xa4
#define UNKNOWN_NT51021_A5 0xa5
#define UNKNOWN_NT51021_A6 0xa6
#define UNKNOWN_NT51021_A7 0xa7
#define UNKNOWN_NT51021_A8 0xa8
#define UNKNOWN_NT51021_A9 0xA9
#define UNKNOWN_NT51021_AA 0xaa
#define UNKNOWN_NT51021_AD 0xAD
#define UNKNOWN_NT51021_B4 0xb4
#define UNKNOWN_NT51021_B5 0xb5
#define UNKNOWN_NT51021_B6 0xb6
#define UNKNOWN_NT51021_C0 0xC0
#define UNKNOWN_NT51021_9A 0x9a
#define UNKNOWN_NT51021_90 0x90
#define UNKNOWN_NT51021_91 0x91
#define UNKNOWN_NT51021_94 0x94
#define UNKNOWN_NT51021_95 0x95
#define UNKNOWN_NT51021_96 0x96
#define UNKNOWN_NT51021_99 0x99

static inline
struct huawei_nt51021 *to_huawei_nt51021(struct drm_panel *panel)
{
	return container_of(panel, struct huawei_nt51021, panel);
}

static void huawei_nt51021_reset(struct huawei_nt51021 *ctx)
{
	gpiod_direction_output(ctx->reset_gpio, 0);
	msleep(200);
	//usleep_range(1000, 2000);
	gpiod_direction_output(ctx->reset_gpio, 1);
	msleep(200);
	gpiod_direction_output(ctx->reset_gpio, 0);
	msleep(300);
}

static void huawei_nt51021_power(struct huawei_nt51021 *ctx, int enable)
{
	gpiod_direction_output(ctx->disp_power_panel, enable);
	usleep_range(300000, 350000);
	gpiod_direction_output(ctx->disp_power_backlight, enable);
	usleep_range(15000, 25000);
}

static void huawei_nt51021_regulator_power_on(struct huawei_nt51021 *ctx)
{
	regulator_enable(ctx->vddio);
	msleep(30);
	regulator_enable(ctx->vsp);
	usleep_range(5000, 5100);
	regulator_enable(ctx->vsn);
	usleep_range(3000, 3500);
}

static void huawei_nt51021_regulator_power_off(struct huawei_nt51021 *ctx)
{
	regulator_enable(ctx->vddio);
	msleep(30);
	regulator_enable(ctx->vsp);
	usleep_range(5000, 5100);
	regulator_enable(ctx->vsn);
	usleep_range(3000, 3500);
}

static int huawei_nt51021_on(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_DATA_SEND, 0xA5);
	mdelay(5);

	ret = mipi_dsi_dcs_soft_reset(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to do Software Reset (%d)\n", ret);
	}
	usleep_range(19000, 21000);

	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_DATA_SEND, 0xA5);
	mdelay(1);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0xaa);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x11);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_IC_MIPI_RX, 0x4b);
	mdelay(10);
	//mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_TESTMODE1, 0x04);
	//mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_TESTMODE2, 0x08);
	//mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_TESTMODE3, 0x10);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0xbb);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x22);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_96, 0x00); //02 on lenovo
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_CABC, 0xc0);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_91, 0xa0);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_9A, 0x10);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_94, 0x78);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_95, 0xb1);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A1, 0xff);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A2, 0xfa);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A3, 0xf3);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A4, 0xed);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A5, 0xe7);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A6, 0xe2);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A7, 0xdc);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A8, 0xd7);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_A9, 0xd1);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_AA, 0xcc);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_B4, 0x1c);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_B5, 0x38);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_B6, 0x30);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x00);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_GOP, 0x80);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_3DUMMY, 0x6c);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_GCH, 0xfc);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_BACKLIGHT, 0x66);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_RESISTOR, 0x00);
	//mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0x00);
	//mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x00);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0xaa);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x11);

	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to exit sleep mode: %d\n", ret);
		return ret;
	}

	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_DATA_SEND, 0x00);
	mdelay(5);

	return 0;
}
static int huawei_nt51021_panel_on(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &ctx->dsi->dev;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret) {
		dev_err(dev, "failed to turn display on (%d)\n", ret);
		return ret;
	}
	usleep_range(80000, 85000);

	return 0;
}
static int huawei_nt51021_off(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &ctx->dsi->dev;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_off(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to set display OFF (%d)\n", ret);
		return ret;
	}
	msleep(80);

	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_DATA_SEND, 0xA5);
	/*mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0xbb);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x22);
	mipi_dsi_dcs_write_seq(dsi, UNKNOWN_NT51021_96, 0x00);*/
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE1, 0x00);
	mipi_dsi_dcs_write_seq(dsi, NOVATEK_NT51021_PAGE2, 0x00);
	//mipi_dsi_dcs_write_seq(dsi, 0x10, 0x00);
	ret = mipi_dsi_dcs_enter_sleep_mode(ctx->dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to enter sleep mode (%d)\n", ret);
		return ret;
	}
	msleep(100);

	return 0;
}

static int huawei_nt51021_pre_prepare(struct huawei_nt51021 *ctx)//, unsigned int inversion_mode)
{
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &ctx->dsi->dev;
	int ret;

	huawei_nt51021_power(ctx, 1);
	msleep(1);

	huawei_nt51021_regulator_power_on(ctx);
	usleep_range(1000, 5000);

	mipi_dsi_dcs_nop(dsi);
	usleep_range(1000, 2000);

	huawei_nt51021_reset(ctx);
	msleep(10);

	ret = huawei_nt51021_on(ctx);

	ret = huawei_nt51021_panel_on(ctx);

	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		huawei_nt51021_power(ctx, 0);
		huawei_nt51021_regulator_power_off(ctx);
		usleep_range(1000, 5000);	
		return ret;
	}

	ret = mipi_dsi_dcs_set_tear_on(dsi, MIPI_DSI_DCS_TEAR_MODE_VBLANK);
	if (ret) {
		dev_err(dev, "failed to set tear on (%d)\n", ret);
		return ret;
	}

	gpiod_direction_output(ctx->disp_en_gpio_vled, 1);
	msleep(100);

	return 0;
}

static int huawei_nt51021_prepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &ctx->dsi->dev;
	int ret;

	mutex_lock(&ctx->mutex);
	huawei_nt51021_pre_prepare(ctx);//,1);
	mutex_unlock(&ctx->mutex);

	return 0;
}
static int huawei_nt51021_unprepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = huawei_nt51021_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value(ctx->reset_gpio, 1);
	huawei_nt51021_power(ctx, 0);
	msleep(1);
	huawei_nt51021_regulator_power_off(ctx);
	usleep_range(1000, 5000);
	gpiod_direction_output(ctx->disp_en_gpio_vled, 0);
	msleep(200);
	//huawei_nt51021_power_off(ctx);
	//regulator_disable(ctx->lcd_bias);
	//msleep(200);

	return 0;
}

static const struct drm_display_mode huawei_nt51021_mode = {
	.clock = (1200 + 64 + 4 + 36) * (1920 + 104 + 2 + 24) * 60 / 1000,
	.hdisplay = 1200,
	.hsync_start = 1200 + 64,
	.hsync_end = 1200 + 64 + 4,
	.htotal = 1200 + 64 + 4 + 36,
	.vdisplay = 1920,
	.vsync_start = 1920 + 104,
	.vsync_end = 1920 + 104 + 2,
	.vtotal = 1920 + 104 + 2 + 24,
	.width_mm = 135,
	.height_mm = 217,
};

static int huawei_nt51021_get_modes(struct drm_panel *panel,
					  struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &huawei_nt51021_mode);
	if (!mode)
		return -ENOMEM;

	drm_mode_set_name(mode);

	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	connector->display_info.width_mm = mode->width_mm;
	connector->display_info.height_mm = mode->height_mm;
	drm_mode_probed_add(connector, mode);

	return 1;
}

static const struct drm_panel_funcs huawei_nt51021_panel_funcs = {
	.prepare = huawei_nt51021_prepare,
	.unprepare = huawei_nt51021_unprepare,
	.get_modes = huawei_nt51021_get_modes,
};

static int huawei_nt51021_set_brightness(struct mipi_dsi_device *dsi,
					u16 brightness)
{
	const u8 chang_page0_index0[2] = {0x83, 0x00};
	const u8 chang_page0_index1[2] = {0x84, 0x00};
	u8 payload[2] = { brightness & 0xff, brightness >> 8 };
	int ret;

	mipi_dsi_dcs_write_buffer(dsi, chang_page0_index0,
					ARRAY_SIZE(chang_page0_index0));

	mipi_dsi_dcs_write_buffer(dsi, chang_page0_index1,
					ARRAY_SIZE(chang_page0_index1));

	ret = mipi_dsi_dcs_write(dsi, NOVATEK_NT51021_BACKLIGHT,
				 payload, sizeof(payload));
	if (ret < 0)
		return ret;

	return 0;
}

static int huawei_nt51021_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	//struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	//struct device *dev = &ctx->dsi->dev;
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	//gpiod_set_value(ctx->disp_en_gpio_vled, !!brightness);
	//msleep(200);
	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = huawei_nt51021_set_brightness(dsi, brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return 0;
}

static const struct backlight_ops huawei_nt51021_bl_ops = {
	.update_status = huawei_nt51021_bl_update_status,
};

static struct backlight_device *
huawei_nt51021_create_backlight(struct mipi_dsi_device *dsi)
{
	//struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.power = FB_BLANK_UNBLANK,
		.type = BACKLIGHT_RAW,
		.brightness = 255,	
		//.brightness = huawei_nt51021_get_actual_brightness(ctx),
		.max_brightness = 255,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &huawei_nt51021_bl_ops, &props);
}

static int huawei_nt51021_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct huawei_nt51021 *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	/*ctx->vddio = devm_regulator_get(dev, "vddio");
	if (IS_ERR(ctx->vddio))
		return PTR_ERR(ctx->vddio);

	ctx->lcd_bias = devm_regulator_get(dev, "bias");
	if (IS_ERR(ctx->lcd_bias))
		return PTR_ERR(ctx->lcd_bias);*/

	ctx->vddio = devm_regulator_get(dev, "vddio");
	if (IS_ERR(ctx->vddio))
		return PTR_ERR(ctx->vddio);

	ctx->vsp = devm_regulator_get(dev, "vsp");
	if (IS_ERR(ctx->vsp))
		return PTR_ERR(ctx->vsp);

	ctx->vsn = devm_regulator_get(dev, "vsn");
	if (IS_ERR(ctx->vsn))
		return PTR_ERR(ctx->vsn);

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->disp_en_gpio_vled = devm_gpiod_get(dev, "backlight", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->disp_en_gpio_vled))
		return dev_err_probe(dev, PTR_ERR(ctx->disp_en_gpio_vled),
				     "Failed to get backlight-gpios\n");
				     
	ctx->disp_power_panel = devm_gpiod_get(dev, "power", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->disp_power_panel))
		return dev_err_probe(dev, PTR_ERR(ctx->disp_power_panel),
				     "Failed to get power-gpios\n");

	ctx->disp_power_backlight = devm_gpiod_get(dev, "blpower", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->disp_power_backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->disp_power_backlight),
				     "Failed to get bklpower-gpios\n");

	ctx->dsi = dsi; 
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST |
			  MIPI_DSI_MODE_VIDEO_HSE | MIPI_DSI_MODE_NO_EOT_PACKET;// |
			  //MIPI_DSI_CLOCK_NON_CONTINUOUS;

	drm_panel_init(&ctx->panel, dev, &huawei_nt51021_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	ctx->panel.prepare_prev_first = true;

	ctx->panel.backlight = huawei_nt51021_create_backlight(dsi);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to attach to DSI host: %d\n", ret);
		drm_panel_remove(&ctx->panel);
		return ret;
	}

	return 0;
}

static void huawei_nt51021_remove(struct mipi_dsi_device *dsi)
{
	struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id huawei_nt51021_of_match[] = {
	{ .compatible = "huawei,boe-nt51021" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, huawei_nt51021_of_match);

static struct mipi_dsi_driver huawei_nt51021_driver = {
	.probe = huawei_nt51021_probe,
	.remove = huawei_nt51021_remove,
	.driver = {
		.name = "panel-huawei-nt51021",
		.of_match_table = huawei_nt51021_of_match,
	},
};
module_mipi_dsi_driver(huawei_nt51021_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for huawei_nt51021_VIDEO");
MODULE_LICENSE("GPL");
