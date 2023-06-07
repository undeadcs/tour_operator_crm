<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph[1]">
		<h1>Группа <xsl:value-of select="name/text()"/>(<xsl:value-of select="*[1]/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name/text()"/></p>
		<p><b>Направление</b>&#160; <xsl:value-of select="direct_txt/text()"/></p>
		<p><b>Комментарий</b>&#160; <xsl:value-of select="comment/text()"/></p>
		<h3>Туры</h3>
		<ul>
		<xsl:for-each select="tour_list/*">
			<li><xsl:value-of select="name/text()"/></li>
		</xsl:for-each>
		</ul>
		<h3>Периоды</h3>
		<ul>
		<xsl:for-each select="period_list/*">
			<li><xsl:value-of select="name/text()"/></li>
		</xsl:for-each>
		</ul>
		<h3>Шаблоны туров</h3>
		<ul>
		<xsl:for-each select="tpltour_list/*">
			<li><xsl:value-of select="name/text()"/></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>