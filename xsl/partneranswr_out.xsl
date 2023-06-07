<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Ответ партнера (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Партнер</b>&#160; <xsl:value-of select="partner_txt/text()"/></p>
		<p><b>Дата</b>&#160; <xsl:value-of select="substring(created/text(),0,10)"/></p>
		<p><b>Текст</b>&#160; <xsl:value-of select="text/text()"/></p>
	</xsl:template>
</xsl:stylesheet>