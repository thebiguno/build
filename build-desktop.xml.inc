	<target name="osx" depends="jar" description="Builds an OSX package, using JarBundler.  If on a Mac, will also wrap this in a .dmg; otherwise, will wrap in .zip">
		<taskdef name="jarbundler" classpath="${JARBUNDLER_JAR}" classname="net.sourceforge.jarbundler.JarBundler"/>

		<mkdir dir="${BUILD}/${FULL_NAME}"/>
		<jarbundler dir="${BUILD}/${FULL_NAME}" verbose="true" showPlist="false"
	        name="${FULL_NAME}"
	        shortname="${FULL_NAME}"
	        signature="????"
	        stubfile="${ETC}/osx/JavaApplicationStub"
	        mainclass="${MAIN_CLASS}"
	        icon="${IMG}/osx/${FULL_NAME}.icns"
	        jvmversion="1.5+"
	        version="${VERSION}"
	        infostring="${FULL_NAME}, ${VERSION}"
	        bundleid="${MAIN_CLASS}"
			vmoptions="${VM_ARGS}">

			<jarfileset file="${BUILD}/${FULL_NAME}.jar" />
			<jarfileset dir="${LIB}">
				<include name="*quaqua*"/>
			</jarfileset>

			<!-- Associate document types with this application.  
			     Unfortunately, I can't figure out a way to 
			     abstract this functionality into a configuration
			     file or the properties at the top of the build
			     script - if someone knows how to do this, so that
			     I can use a single build script for multiple 
			     applications and only change the properties, 
			     please let me know!
			-->
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

		</jarbundler>

		<copy todir="${BUILD}/${FULL_NAME}">
			<fileset dir="${DOCS}">
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

		<delete dir="${BUILD}/${FULL_NAME}"/>

	</target>
	
	
	<target name="osx_dmg" if="OS_IS_MAC" description="Bundles the Macintosh .app package in a .zip.  Automatically called from target osx if you are building on a Mac.">
		<exec executable="/usr/bin/hdiutil" os="Mac OS X">
			<arg line="create -format UDZO -srcfolder '${BUILD}/${FULL_NAME}' -o '${BUILD}/${SAFE_NAME}-${VERSION}.dmg'"/>
		</exec>
	</target>

	<target name="osx_zip" unless="OS_IS_MAC" description="Bundles the Macintosh .app package in a .zip.  Automatically called from target osx if you are building on something other than a Mac.">
		<zip destfile="${BUILD}/${SAFE_NAME}-${VERSION}-OSX.zip">
			<fileset dir="${BUILD}/${FULL_NAME}"/>
		</zip>
	</target>