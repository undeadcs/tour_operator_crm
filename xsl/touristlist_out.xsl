<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph[1]">
		<h1>Список туристов <xsl:value-of select="name/text()"/>(<xsl:value-of select="*[1]/text()"/>)</h1>
		<p><b>Дата выезда</b>&#160; <xsl:value-of select="substring(date1/text(),0,10)"/></p>
		<p><b>Дата въезда</b>&#160; <xsl:value-of select="substring(date2/text(),0,10)"/></p>
		<p><b>Пункт въезда</b>&#160; <xsl:value-of select="exit_txt"/></p>
		<p><b>Пункт выезда</b>&#160; <xsl:value-of select="enter_txt"/></p>
		<p><b>Принимающая сторона</b>&#160; <xsl:value-of select="host_txt"/></p>
		<p><b>Маршрут</b>&#160; <xsl:value-of select="route_txt"/></p>
		<p><b>Руководитель группы</b>&#160; <xsl:value-of select="leader_txt"/></p>
		<h3>Туристы</h3>
		<ul>
		<xsl:for-each select="tourist_list/*">
			<li><xsl:value-of select="text()"/></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>