<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.5.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="Flex">
<packages>
<package name="FLEX6">
<description>WR-FPC 1mm SMT Horiz. Bot. Contact Hinge type WR-FPC 1mm 06pins Hinge type SMT Horizontal Bottom Contact</description>
<smd name="1" x="-2.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<smd name="2" x="-1.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<smd name="3" x="-0.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<smd name="4" x="0.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<smd name="5" x="1.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<smd name="6" x="2.5" y="0.25" dx="0.6" dy="2" layer="1"/>
<wire x1="-3.3" y1="-1.25" x2="-3.3" y2="1.62" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.62" x2="3.3" y2="1.62" width="0.127" layer="21"/>
<wire x1="3.3" y1="1.62" x2="3.3" y2="-1.25" width="0.127" layer="21"/>
<wire x1="3.3" y1="-1.25" x2="-3.3" y2="-1.25" width="0.127" layer="21"/>
<text x="-3.8" y="-3.028" size="1.27" layer="27">&gt;VALUE</text>
<text x="-3.8" y="1.95" size="1.27" layer="25">&gt;NAME</text>
<circle x="-3" y="-0.9" radius="0.1" width="0.1524" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="SINGLEROW-6">
<pin name="1" x="0" y="0" visible="pad" length="short" direction="pas" swaplevel="1"/>
<pin name="2" x="0" y="-2.54" visible="pad" length="short" direction="pas" swaplevel="1"/>
<pin name="3" x="0" y="-5.08" visible="pad" length="short" direction="pas" swaplevel="1"/>
<pin name="4" x="0" y="-7.62" visible="pad" length="short" direction="pas" swaplevel="1"/>
<pin name="5" x="0" y="-10.16" visible="pad" length="short" direction="pas" swaplevel="1"/>
<pin name="6" x="0" y="-12.7" visible="pad" length="short" direction="pas" swaplevel="1"/>
<text x="0" y="3" size="1.778" layer="95">&gt;NAME</text>
<text x="0" y="-17.78" size="1.778" layer="96">&gt;VALUE</text>
<wire x1="2.54" y1="2.54" x2="2.54" y2="-15.24" width="0.254" layer="94"/>
<wire x1="2.54" y1="-15.24" x2="10.16" y2="-15.24" width="0.254" layer="94"/>
<wire x1="10.16" y1="-15.24" x2="10.16" y2="2.54" width="0.254" layer="94"/>
<wire x1="10.16" y1="2.54" x2="2.54" y2="2.54" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="FLEX6" prefix="K">
<description>&lt;b&gt;WR-FPC 1mm SMT Horiz. Bot. Contact Hinge type WR-FPC 1mm 06 pins Hinge type SMT Horizontal Bottom Contact
;&lt;/b&gt;=&gt;Code : Con_FPC_ZIF_1.00_6861xx148922_686106148922
&lt;p&gt;&lt;a href="http://katalog.we-online.de/media/images/eican/Con_FPC_ZIF_1.00_686110148922_pf2.jpg"&gt;
&lt;img src="http://katalog.we-online.de/media/thumbs2/eican/thb_Con_FPC_ZIF_1.00_686110148922_pf2.jpg" width="320"&gt;&lt;/a&gt;&lt;p&gt;
Details see: &lt;a href="http://katalog.we-online.de/em/datasheet/6861xx148922.pdf"&gt;http://katalog.we-online.de/em/datasheet/6861xx148922.pdf&lt;/a&gt;&lt;p&gt;
Created 2014-10-08, Karrer Zheng&lt;br&gt;
2014 (C) Wurth Elektronik</description>
<gates>
<gate name="G$1" symbol="SINGLEROW-6" x="0" y="0"/>
</gates>
<devices>
<device name="" package="FLEX6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="K1" library="Flex" deviceset="FLEX6" device=""/>
<part name="K2" library="Flex" deviceset="FLEX6" device=""/>
<part name="K3" library="Flex" deviceset="FLEX6" device=""/>
<part name="K4" library="Flex" deviceset="FLEX6" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="K1" gate="G$1" x="27.94" y="48.26" smashed="yes" rot="MR270">
<attribute name="NAME" x="24.94" y="48.26" size="1.778" layer="95" rot="MR270"/>
</instance>
<instance part="K2" gate="G$1" x="27.94" y="73.66" smashed="yes" rot="R90">
<attribute name="NAME" x="24.94" y="73.66" size="1.778" layer="95" rot="R90"/>
</instance>
<instance part="K3" gate="G$1" x="63.5" y="48.26" smashed="yes" rot="MR270">
<attribute name="NAME" x="60.5" y="48.26" size="1.778" layer="95" rot="MR270"/>
</instance>
<instance part="K4" gate="G$1" x="63.5" y="73.66" smashed="yes" rot="R90">
<attribute name="NAME" x="60.5" y="73.66" size="1.778" layer="95" rot="R90"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="K1" gate="G$1" pin="1"/>
<pinref part="K2" gate="G$1" pin="1"/>
<wire x1="27.94" y1="48.26" x2="27.94" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="K2" gate="G$1" pin="2"/>
<pinref part="K1" gate="G$1" pin="2"/>
<wire x1="30.48" y1="73.66" x2="30.48" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="K1" gate="G$1" pin="3"/>
<pinref part="K2" gate="G$1" pin="3"/>
<wire x1="33.02" y1="48.26" x2="33.02" y2="73.66" width="0.1524" layer="91"/>
<label x="33.02" y="60.96" size="1.778" layer="95" rot="R90"/>
</segment>
<segment>
<pinref part="K4" gate="G$1" pin="4"/>
<pinref part="K3" gate="G$1" pin="4"/>
<wire x1="71.12" y1="73.66" x2="71.12" y2="48.26" width="0.1524" layer="91"/>
<label x="71.12" y="60.96" size="1.778" layer="95" rot="R90"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="K2" gate="G$1" pin="4"/>
<pinref part="K1" gate="G$1" pin="4"/>
<wire x1="35.56" y1="73.66" x2="35.56" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="K1" gate="G$1" pin="5"/>
<pinref part="K2" gate="G$1" pin="5"/>
<wire x1="38.1" y1="48.26" x2="38.1" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="K1" gate="G$1" pin="6"/>
<pinref part="K2" gate="G$1" pin="6"/>
<wire x1="40.64" y1="48.26" x2="40.64" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
