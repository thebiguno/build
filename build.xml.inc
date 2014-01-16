	<property name="PROJECT_HOME" value = "."/>
	<exec outputproperty="GIT_VERSION" executable="git">
		<arg value="describe"/>
		<arg value="--long"/>
		<arg value="--tags"/>
	</exec>

	<tstamp>
		<format property="TIMESTAMP" pattern="yyyyMMddHHmm"/>
	</tstamp>
	
	<target name="javadocs" depends="clean" description="Creates the javadocs">
		<mkdir dir="${PROJECT_HOME}/build/javadocs"/>
		<javadoc destdir="${PROJECT_HOME}/build/javadocs" 
				packagenames="org"
				classpathref="classpath">
			<packageset dir="${PROJECT_HOME}/src">
				<include name="**/*"/>
			</packageset>
		</javadoc>
	</target>
	
	<target name="release" depends="jar,javadocs" description="Build a release package including jar, javadocs, and changelog">
		<zip destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${GIT_VERSION}.zip">
			<zipfileset prefix="lib" dir="${PROJECT_HOME}/build/dist">
				<include name="*.jar"/>
			</zipfileset>
			<zipfileset prefix="javadocs" dir="${PROJECT_HOME}/build/javadocs"/>
			<zipfileset prefix="docs" dir="${PROJECT_HOME}/docs/">
				<include name="*.txt"/>
			</zipfileset>
		</zip>
	</target> 

	<target name="jar" depends="clean,compile" description="Jar the project">
		<condition property="is-taglib-project">
			<available file="${PROJECT_HOME}/WebContent/WEB-INF/tlds" type="dir"/>
		</condition>
		
		<antcall target="jar-standard"/>
		<antcall target="jar-taglib"/>
	</target>

	<target name="war" depends="jar" description="War the project">
		<mkdir dir="${PROJECT_HOME}/build/temp/classes"/>
		<filter token="environment" value="${PROJECT_SHORT_NAME}"/>
		<copy filtering="true" todir="${PROJECT_HOME}/build/temp/classes" overwrite="true">
			<fileset dir="${PROJECT_HOME}/WebContent/WEB-INF/classes"/>
		</copy>
		
		<mkdir dir="${PROJECT_HOME}/build/dist"/>
		<war destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}.war" webxml="${PROJECT_HOME}/WebContent/WEB-INF/web.xml">
			<classes dir="${PROJECT_HOME}/build/temp/classes"/>
			<lib dir="${PROJECT_HOME}/WebContent/WEB-INF/lib">
				<include name="*.jar"/>
			</lib>
			<lib dir="${PROJECT_HOME}/build/dist">
				<include name="${PROJECT_SHORT_NAME}-${GIT_VERSION}.jar"/>
			</lib>
			<zipfileset dir="${PROJECT_HOME}/WebContent">
				<exclude name="WEB-INF/classes/**"/>
				<exclude name="WEB-INF/lib/**"/>
			</zipfileset>
		</war>
	</target>

	<target name="jar-standard" unless="is-taglib-project">
		<mkdir dir="${PROJECT_HOME}/build/dist"/>
		<mkdir dir="${PROJECT_HOME}/build/temp/META-INF"/>
		<filter token="QUALIFIER" value="${TIMESTAMP}"/>
		<copy filtering="true" todir="build/temp/META-INF" overwrite="true">
			<fileset dir="${PROJECT_HOME}/META-INF">
				<include name="MANIFEST.MF"/>
			</fileset>
		</copy>

		<jar destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${GIT_VERSION}.jar" filesetmanifest="merge">
			<fileset dir="${PROJECT_HOME}/build/classes" includes="**/*"/>
			<fileset dir="${PROJECT_HOME}/src" includes="**/*"/>
			<fileset dir="${PROJECT_HOME}/build/temp" includes="META-INF/MANIFEST.MF"/>
		</jar>
	</target>
	
	<target name="jar-taglib" if="is-taglib-project">
		<mkdir dir="${PROJECT_HOME}/build/dist"/>
		<jar destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${GIT_VERSION}.jar">
			<fileset dir="${PROJECT_HOME}/build/classes" includes="**/*"/>
			<fileset dir="${PROJECT_HOME}/src" includes="**/*"/>
			<metainf dir="${PROJECT_HOME}/WebContent/WEB-INF/tlds">
				<include name="*.tld"/>
			</metainf>
		</jar>
	</target>

	<target name="compile" depends="clean" description="Builds the project">
		<mkdir dir="${PROJECT_HOME}/build/classes"/>
		<javac srcdir="src" 
			target="1.5"
			source="1.5" 
			optimize="true" 
			includes="**/*" 
			destdir="build/classes" 
			debug="true" 
			debuglevel="lines,vars,source" 
			classpathref="classpath">
			<exclude name="${COMPILE_EXCLUDES}"/>
		</javac>
	</target>

	<target name="clean" description="Removes all files from build directory">
		<delete includeemptydirs="true" failonerror="false">
			<fileset dir="${PROJECT_HOME}/build">
				<include name="**/*"/>
			</fileset>
		</delete>
	</target>
