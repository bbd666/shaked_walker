function write_params_to_xml(v)
    InitialCondition_l ={num2str(v(1)),num2str(v(2)),num2str(v(3)),num2str(v(4)),num2str(v(5)),num2str(v(6)),num2str(v(7))};
    StanceLead_l ={num2str(v(8)), num2str(v(9)),num2str(v(10)),num2str(v(11)), num2str(v(12)) ,num2str(v(13)), num2str(v(14)), num2str(v(15)), num2str(v(16))};
    ForceFeedback_l = {num2str(v(17)), num2str(v(18)), num2str(v(19)), num2str(v(20)), num2str(v(21)), num2str(v(22))};
    LengthFeedback_l = {num2str(v(23)), num2str(v(24)), num2str(v(25)), num2str(v(26)), num2str(v(27)), num2str(v(28))};
    Coronal_lead_l = {num2str(v(29)), num2str(v(30)), num2str(v(31)), num2str(v(32))};
    Coronal_l = {num2str(v(33)), num2str(v(34)), num2str(v(35)), num2str(v(36)), num2str(v(37)), num2str(v(38))};
    Arm_l = {num2str(v(39)), num2str(v(40))};


    InitialCondition_v ={'trunk','alfaR','alfaL','beta','gamma','head','vel'};
    StanceLead_v ={'hamP', 'hamA','hamD','gluP', 'gluA' ,'gluD', 'hflP', 'hflA', 'hflD'};
    ForceFeedback_v = {'glu', 'ham', 'vas', 'sol', 'gas', 'tasol'};
    LengthFeedback_v = {'hflG', 'hamG', 'taG', 'hflH', 'hamH', 'taH'};
    Coronal_lead_v = {'trunkP', 'trunkV', 'footP', 'footV'};
    Coronal_v = {'trunkP', 'trunkV', 'footP', 'footV', 'TtrunkP', 'TtrunkV'};
    Arm_v = {'alpha', 'elbow'};

    docNode = com.mathworks.xml.XMLUtils.createDocument('Params');
    Params = docNode.getDocumentElement;
    Params.setAttribute('version','1.0');

    Params = create_set(docNode,Params,'InitialCondition',InitialCondition_v,InitialCondition_l);
    Params = create_set(docNode,Params,'StanceLead',StanceLead_v,StanceLead_l);
    Params = create_set(docNode,Params,'ForceFeedback',ForceFeedback_v,ForceFeedback_l);
    Params = create_set(docNode,Params,'LengthFeedback',LengthFeedback_v,LengthFeedback_l);
    Params = create_set(docNode,Params,'Coronal_lead',Coronal_lead_v,Coronal_lead_l);
    Params = create_set(docNode,Params,'Coronal',Coronal_v,Coronal_l);
    Params = create_set(docNode,Params,'Arm',Arm_v,Arm_l);

    xmlwrite('OptimizationParameters.xml',docNode);

end

function Params = create_set(docNode,Params,element, v, l)
    set = docNode.createElement(element);
    for kk = 1:length(v)
        set.setAttribute(v{kk},l{kk});
    end
    Params.appendChild(set);
end