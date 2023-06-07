<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Партнер (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name/text()"/></p>
		<p><b>ФИО директора</b>&#160; <xsl:value-of select="dfio/text()"/></p>
		<p><b>ФИО контактного лица</b>&#160; <xsl:value-of select="cfio/text()"/></p>
		<p><b>E-mail</b>&#160; <xsl:value-of select="email/text()"/></p>
		<p><b>Телефон</b>&#160; <xsl:value-of select="phone/text()"/></p>
		<p><b>Факс</b>&#160; <xsl:value-of select="fax/text()"/></p>
		<p><b>Комментарий</b>&#160; <xsl:value-of select="comment/text()"/></p>
	</xsl:template>
</xsl:stylesheet>