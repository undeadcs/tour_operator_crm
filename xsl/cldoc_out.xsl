<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Документ клиента (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Тип</b>&#160; <xsl:value-of select="type_txt/text()"/></p>
		<p><b>Серия</b>&#160; <xsl:value-of select="series/text()"/></p>
		<p><b>Номер</b>&#160; <xsl:value-of select="number/text()"/></p>
		<p><b>Дата выдачи</b>&#160; <xsl:value-of select="substring(issuedate/text(),0,10)"/></p>
		<p><b>Дата окончания</b>&#160; <xsl:value-of select="substring(enddate/text(),0,10)"/></p>
		<p><b>Кем выдан</b>&#160; <xsl:value-of select="issuedby/text()"/></p>
		<p><b>Место рождения</b>&#160; <xsl:value-of select="birthplace/text()"/></p>
		<p><b>ФИО</b>&#160; <xsl:value-of select="fio/text()"/></p>
		<p><b>ФИО латиницей</b>&#160; <xsl:value-of select="fiolatin/text()"/></p>
	</xsl:template>
</xsl:stylesheet>