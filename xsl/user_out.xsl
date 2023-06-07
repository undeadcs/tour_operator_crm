<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Пользователь (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Логин</b>&#160; <xsl:value-of select="login/text()"/></p>
		<p><b>E-mail</b>&#160; <xsl:value-of select="email/text()"/></p>
		<p><b>ФИО</b>&#160; <xsl:value-of select="fio/text()"/></p>
		<p><b>Ранг</b>&#160; <xsl:choose>
			<xsl:when test="rank/text()='1'">Администратор</xsl:when>
			<xsl:when test="rank/text()='2'">Менеджер</xsl:when>
			<xsl:when test="rank/text()='3'">Кассир</xsl:when>
			<xsl:when test="rank/text()='4'">Бухгалтер</xsl:when>
			<xsl:when test="rank/text()='5'">Руководитель</xsl:when>
			<xsl:when test="rank/text()='6'">Суперамдинистратор</xsl:when>
			<xsl:otherwise>неизвестный</xsl:otherwise>
		</xsl:choose></p>
		<p><b>Серия</b>&#160; <xsl:value-of select="series/text()"/></p>
	</xsl:template>
</xsl:stylesheet>