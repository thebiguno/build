	<property name="PROJECT_HOME" value = "."/>

	<target name="javadocs" depends="clean" description="Creates the javadocs">
		<mkdir dir="${PROJECT_HOME}/build/javadocs"/>
		<javadoc destdir="${PROJECT_HOME}/build/javadocs" packagenames="org">
			<packageset dir="${PROJECT_HOME}/src">
				<include name="**/*"/>
			</packageset>
		</javadoc>
	</target>

	<target name="jar" depends="clean,compile" description="Jar the project">
		<mkdir dir="${PROJECT_HOME}/build/dist"/>
		<jar destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${VERSION}.jar">
			<fileset dir="${PROJECT_HOME}/build/classes" includes="**/*"/>
		</jar>
	</target>
	
	<target name="jar-taglib" depends="clean,compile" description="Jar the project as a taglib.">
		<mkdir dir="${PROJECT_HOME}/build/dist"/>
		<jar destfile="${PROJECT_HOME}/build/dist/${PROJECT_SHORT_NAME}-${VERSION}.jar">
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
			includes="**/*" 
			destdir="build/classes" 
			debug="true" 
			debuglevel="lines,vars,source" 
			classpathref="classpath"/>
	</target>

	<target name="clean" description="Removes all files from build directory">
		<delete includeemptydirs="true" failonerror="false">
			<fileset dir="${PROJECT_HOME}/build">
				<include name="**/*"/>
			</fileset>
		</delete>
	</target>
