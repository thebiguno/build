<!--
Include this build.xml snippet when you want to build desktop applications, as opposed 
to a simple .jar file.

To take full advantage of this, you must have the following directory structure:

./
	docs/
	etc/
		build.properties (Must include at a minimum 'VERSION=' and 'MAIN-CLASS'.  VM_ARGS and PROGRAM_ARGS are highly recommended.)
	img/
		osx/<PROJECT_SHORT_NAME>.icns
		windows/<PROJECT_SHORT_NAME>.ico
		unix/ (TODO?)
	lib/*.jar   (Cross platform .jars, included in all distributions)
		osx/    (Platform specific .jars / .jnilib, only included in relevant distributions)
		windows/
		linux/
		
This script will try to give meaningful error messages if a file is missing, but there 
are no guarantees 8-)
-->


	<property name="BUILD_TOOLS" value="../ca.digitalcave.build/"/>
	<property name="BUILD_LIB" value="${BUILD_TOOLS}/lib/"/>
	<property name="BUILD_ETC" value="${BUILD_TOOLS}/etc/"/>
	
	<target name="osx" depends="jar" description="Builds an OSX package, using JarBundler.  If on a Mac, will also wrap this in a .dmg; otherwise, will wrap in .zip">
		<taskdef name="jarbundler" classpath="${BUILD_LIB}/jarbundler/jarbundler-1.9.jar" classname="net.sourceforge.jarbundler.JarBundler"/>

		<mkdir dir="${PROJECT_HOME}/build/osx/${PROJECT_NAME}"/>
		<jarbundler dir="${PROJECT_HOME}/build/osx/${PROJECT_NAME}" verbose="true" showPlist="false"
	        name="${PROJECT_NAME}"
	        shortname="${PROJECT_SHORT_NAME}"
	        signature="????"
	        stubfile="${BUILD_ETC}/osx/JavaApplicationStub"
	        mainclass="${MAIN_CLASS}"
	        icon="${PROJECT_HOME}/img/osx/${PROJECT_SHORT_NAME}.icns"
	        jvmversion="1.5+"
	        version="${VERSION}"
	        infostring="${PROJECT_NAME}, ${VERSION}"
	        bundleid="${MAIN_CLASS}"
	        arguments="${PROGRAM_ARGS}"
			vmoptions="${VM_ARGS}">

			<jarfileset file="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${VERSION}.jar" />
			<!-- All cross platform common libraries should go in lib --> 
			<jarfileset dir="${PROJECT_HOME}/lib">
				<include name="*.jar"/>
			</jarfileset>
			<!-- All non-cross platform libraries with associated .jnilib should go in lib/os,
			     where os is one of win|osx|linux etc. -->
			<jarfileset dir="${PROJECT_HOME}/lib/osx">
				<include name="*.jar"/>
				<include name="*.jnilib"/>
			</jarfileset>			 

			<!-- Associate document types with this application.  
			     Unfortunately, I can't figure out a way to 
			     abstract this functionality into a configuration
			     file or the properties at the top of the build
			     script - if someone knows how to do this, so that
			     I can use a single build script for multiple 
			     applications and only change the properties, 
			     please let me know!
			<documenttype name="Buddi Data"
				extensions="buddi3"
				iconFile="${IMG}/osx/BuddiData.icns"
				role="Editor"/>
				                
			<documenttype name="Buddi Plugin"
				extensions="buddi3plugin" 
				iconFile="${IMG}/osx/BuddiPlugin.icns"
				role="Editor"/>
			
			<documenttype name="Buddi Translation"
				extensions="lang" 
				iconFile="${IMG}/osx/BuddiLanguage.icns"
				role="Editor"/>
			-->

		</jarbundler>

		<copy todir="${PROJECT_HOME}/build/osx/${PROJECT_NAME}">
			<fileset dir="${PROJECT_HOME}/docs">
				<include name="*.txt"/>
				<include name="*.rtf"/>
				<include name="Licenses/*"/>
			</fileset>
		</copy>
				
		<condition property="OS_IS_MAC">
			<os name="Mac OS X"/>
		</condition>

		<ant target="osx_zip"/>
		<ant target="osx_dmg"/>

		<delete dir="${PROJECT_HOME}/build/osx"/>

	</target>
	
	
	<target name="osx_dmg" if="OS_IS_MAC" description="Bundles the Macintosh .app package in a .zip.  Automatically called from target osx if you are building on a Mac.">
		<exec executable="/usr/bin/hdiutil" os="Mac OS X">
			<arg line="create -format UDZO -srcfolder '${PROJECT_HOME}/build/osx/${PROJECT_NAME}' -o '${PROJECT_HOME}/build/dist/${PROJECT_NAME}.dmg'"/>
		</exec>
	</target>

	<target name="osx_zip" unless="OS_IS_MAC" description="Bundles the Macintosh .app package in a .zip.  Automatically called from target osx if you are building on something other than a Mac.">
		<zip destfile="${PROJECT_HOME}/build/dist/${PROJECT_NAME}-OSX.zip">
			<fileset dir="${PROJECT_HOME}/build/osx/${PROJECT_SHORT_NAME}"/>
		</zip>
	</target>