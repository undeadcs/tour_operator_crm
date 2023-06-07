<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph[1]">
		<h1>Шаблон тура <xsl:value-of select="name/text()"/>(<xsl:value-of select="*[1]/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name/text()"/></p>
		<p><b>Направление</b>&#160; <xsl:value-of select="direct_txt/text()"/></p>
		<p><b>Продолжительность</b>&#160; <xsl:value-of select="daynum/text()"/></p>
		<h3>Маршрут</h3>
		<ul>
		<xsl:for-each select="route_list/*">
			<li><xsl:value-of select="name/text()"/></li>
		</xsl:for-each>
		</ul>
		<h3>Услуги</h3>
		<ul>
		<xsl:for-each select="service_list/*">
			<li><xsl:value-of select="text()"/></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>