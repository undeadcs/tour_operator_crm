<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Валютная пара (<xsl:value-of select="id/text()"/>)</h1>
		<p><b><xsl:value-of select="currency1_txt/text()"/>/<xsl:value-of select="currency2_txt/text()"/></b></p>
		<p><b>Курс</b>&#160; <xsl:value-of select="course/text()"/></p>
		<p><b>Дата установки</b>&#160; <xsl:value-of select="substring(set/text(),0,10)"/></p>
		<p><b>Внутренний курс</b>&#160; <xsl:value-of select="internal/text()"/></p>
	</xsl:template>
</xsl:stylesheet>