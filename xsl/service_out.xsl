<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph[1]">
		<h1>Услуга <xsl:value-of select="name/text()"/>(<xsl:value-of select="*[1]/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name_txt/text()"/></p>
		<p><b>Тип</b>&#160; <xsl:value-of select="type_txt/text()"/></p>
		<p><b>Страна</b>&#160; <xsl:value-of select="country_txt/text()"/></p>
		<p><b>Город</b>&#160; <xsl:value-of select="city_txt/text()"/></p>
		<p><b>Партнер</b>&#160; <xsl:value-of select="partner_txt/text()"/></p>
		<p><b>Единица измерения</b>&#160; <xsl:value-of select="munit_txt/text()"/></p>
		<h3>Периоды</h3>
		<ul>
		<xsl:for-each select="period_list/*">
			<li><b><xsl:value-of select="index/text()"/></b>, <xsl:value-of select="substring(date1/text(),0,10)"/> &#8212; <xsl:value-of select="substring(date2/text(),0,10)"/></li>
		</xsl:for-each>
		</ul>
		<h3>Цены</h3>
		<ul>
		<xsl:for-each select="price_list/*">
			<li>период: <b><xsl:value-of select="period/text()"/></b>, количество людей: <b><xsl:value-of select="num/text()"/></b>, сумма: <b><xsl:value-of select="amount/text()"/></b></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>