<?php include_once('header.php'); ?>

            <div id="page-wrapper">
                <div class="container-fluid">
                    <div class="row">
                        <div class="col-lg-12">
                            <h1 class="page-header">Simulator</h1>
                        </div>
                        <!-- /.col-lg-12 -->
                    </div>
                    <!-- /.row -->
                    <div class="row">
                        <div class="col-lg-2">
                            <div class="panel panel-primary">
							<div class="panel-heading">
                                </div>
                                    <div class="panel-footer">
                                        <span class="pull-left"><button onclick="myFunction()">Run</button></span>
                                        <div class="clearfix"></div>
                                    </div>
                            </div>
                        </div>
						<div class="col-lg-2">
                            <div class="panel panel-green">
							<div class="panel-heading">
                                </div>
                                    <div class="panel-footer">
                                        <span class="pull-left"><button onclick="myFunction()1">Step</button></span>
                                        <div class="clearfix"></div>
                                    </div>
                            </div>
                        </div>
						<div class="col-lg-2">
                            <div class="panel panel-red">
							<div class="panel-heading">
                                </div>
                                    <div class="panel-footer">
                                        <span class="pull-left"><button onclick="myFunction2()">Previous</button></span>
                                        <div class="clearfix"></div>
                                    </div>
                            </div>
                        </div>
						<div class="col-lg-2">
                            <div class="panel panel-primary">
								<div class="panel-heading">
                                </div>
                                    <div class="panel-footer">
                                        <span class="pull-left"><button onclick="myFunction3()">Reset</button></span>
                                        <div class="clearfix"></div>
                                    </div>
                            </div>
                        </div>
                    </div>
					
					<!-- /.row -->
                        <!-- /.col-lg-8 -->
                        <div class="col-lg-3">
                            <div class="panel panel-default">
                                <div class="panel-heading"> 
									<b>MACHINE CODE</b>
                                </div>
                                <!-- /.panel-heading -->
                                <div class="panel-body">
                                    <div class="list-group">
										<table>
										<th>
										PC   Machine Code
										</th>
										<?php
										$myfile = fopen("Machine_Code.txt", "r") or die("Unable to open file!");
							
										while(!feof($myfile)) {?>
										<tr class="list-group-item">
										<?php
										echo fgets($myfile) . "<br>";?>
										</tr> 
										<?php }
										fclose($myfile);
										?>   
										</table>
                                    </div>
                                    <!-- /.list-group -->
                                    
                                </div>
                                <!-- /.panel-body -->
                            </div>
                            <!-- /.panel -->
                            <!-- /.panel .chat-panel -->
                        </div>
						<div class="col-lg-3">
                            <div class="panel panel-default">
                                <div class="panel-heading"> 
									<b>BASIC CODE</b>
                                </div>
                                <!-- /.panel-heading -->
                                <div class="panel-body">
                                    <div class="list-group">
										<?php
										$myfile = fopen("Basic_Code.txt", "r") or die("Unable to open file!");
							
										while(!feof($myfile)) {?>
										<a class="list-group-item">
										<?php
										echo fgets($myfile) . "<br>";?>
										</a> 
										<?php }
										fclose($myfile);
										?>   
                                    </div>
                                    <!-- /.list-group -->
                                    
                                </div>
                                <!-- /.panel-body -->
                            </div>
                            <!-- /.panel -->
                            <!-- /.panel .chat-panel -->
                        </div>
						<div class="row">
						<div class="col-lg-3">
                            <div class="panel panel-default">
                                <div class="panel-heading"> 
									<b>ORIGINAL CODE</b>
                                </div>
                                <!-- /.panel-heading -->
                                <div class="panel-body">
                                    <div class="list-group">
										<?php
										$myfile = fopen("Original_Code.txt", "r") or die("Unable to open file!");
							
										while(!feof($myfile)) {?>
										<a class="list-group-item">
										<?php
										echo fgets($myfile) . "<br>";?>
										</a> 
										<?php }
										fclose($myfile);
										?>   
                                    </div>
                                    <!-- /.list-group -->
                                    
                                </div>
                                <!-- /.panel-body -->
                            </div>
                            <!-- /.panel -->
                            <!-- /.panel .chat-panel -->
                        <!-- /.col-lg-4 -->
                    </div>
                    <!-- /.row -->
                </div>
                <!-- /.container-fluid -->
            </div>
            <!-- /#page-wrapper -->

        </div>
        <!-- /#wrapper -->
<script>
function myFunction()
{
}
function myFunction1() {
  
}
function myFunction2() {
 
}
function myFunction3() {
 
}
</script>

<?php include_once('footer.php'); ?>
